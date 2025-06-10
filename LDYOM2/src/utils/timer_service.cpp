#include "timer_service.h"
#include "string_utils.h"
#include <CTheScripts.h>
#include <CUserDisplay.h>
#include <cassert>
#include <extensions/ScriptCommands.h>
#include <stdexcept>

using namespace plugin;

TimerService::Timer::Timer(std::uintptr_t timerAddress) noexcept
	: timerAddress_(timerAddress) {}

TimerService::Timer::~Timer() noexcept {
	try {
		CUserDisplay::OnscnTimer.ClearClock(timerAddress_);
	} catch (...) {
	}
}

std::uintptr_t TimerService::getScriptSpacePtr() noexcept {
	return reinterpret_cast<std::uintptr_t>(CTheScripts::ScriptSpace);
}

std::uintptr_t TimerService::getTimerAddress() noexcept { return getScriptSpacePtr() + TIMER_OFFSET; }

void TimerService::addTimer(std::string_view text, bool isDown, int value) {
	if (timerActive_) {
		removeTimer();
	}

	const auto timerAddress = getTimerAddress();

	// Set timer value
	auto& timerValue = *reinterpret_cast<int*>(timerAddress);
	timerValue = value;

	currentTimer_ = std::make_unique<Timer>(TIMER_OFFSET);

	auto cp1251Text = utf8_to_cp1251(text);
	gxt_encode(cp1251Text);
	Command<0x0ADF>("LDTIMER", cp1251Text.c_str()); // ADD_TEXT_LABEL

	CUserDisplay::OnscnTimer.AddClock(TIMER_OFFSET, (char*)"LDTIMER", isDown);

	timerActive_ = true;
}

void TimerService::removeTimer() noexcept {
	if (!timerActive_) {
		return;
	}

	currentTimer_.reset();
	timerActive_ = false;
}

int TimerService::getTimerTime() const noexcept {
	if (!timerActive_) {
		return 0;
	}

	const auto timerAddress = getTimerAddress();
	return *reinterpret_cast<const int*>(timerAddress);
}
