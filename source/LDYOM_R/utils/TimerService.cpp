#include "TimerService.h"

#include <CTheScripts.h>
#include <CUserDisplay.h>

uintptr_t spaceVarsPtr = NULL;
uintptr_t timer = NULL;

bool& TimerService::isTimerOn() {
	return timerOn_;
}

void TimerService::addTimer(const char *text, const bool direction, const int value) {
	spaceVarsPtr = reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace);
	timer = 12640;

	if (this->isTimerOn())
		return;

	auto &address = *reinterpret_cast<int*>(spaceVarsPtr + timer);
	address = value;

	CUserDisplay::OnscnTimer.AddClock(timer, const_cast<char*>(text), direction);

	this->timerOn_ = true;
}

void TimerService::removeTimer() {
	CUserDisplay::OnscnTimer.ClearClock(timer);
	this->timerOn_ = false;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
int TimerService::getTimerTime() {
	return *reinterpret_cast<int*>(spaceVarsPtr + timer);
}
