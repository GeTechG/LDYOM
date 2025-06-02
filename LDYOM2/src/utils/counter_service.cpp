#include "counter_service.h"

#include "string_utils.h"
#include <CText.h>
#include <CTheScripts.h>
#include <CUserDisplay.h>
#include <array>
#include <extensions/ScriptCommands.h>
#include <optional>

using namespace plugin;

namespace {
constexpr std::array<const char*, 4> FTX_TEXTS = {"LDCNTR0", "LDCNTR1", "LDCNTR2", "LDCNTR3"};

constexpr uintptr_t COUNTER_BASE_OFFSET = 12644;
constexpr size_t COUNTER_SIZE = 4;
} // namespace

constexpr const char* indexToFtxText(const size_t index) noexcept {
	return (index < FTX_TEXTS.size()) ? FTX_TEXTS[index] : "";
}

namespace {
std::optional<size_t> findAvailableCounterSlot(const std::array<bool, 4>& usedCounters) noexcept {
	for (size_t i = 0; i < usedCounters.size(); ++i) {
		if (!usedCounters[i]) {
			return i;
		}
	}
	return std::nullopt;
}

bool isValidLineId(const size_t lineId, const std::array<bool, 4>& usedCounters) noexcept {
	return lineId < usedCounters.size() && !usedCounters[lineId];
}

void setupTextLabel(const std::string& title, const size_t lineId) {
	auto cp1251Text = utf8_to_cp1251(title);
	gxt_encode(cp1251Text);
	Command<0x0ADF>(indexToFtxText(lineId), cp1251Text.c_str()); // ADD_TEXT_LABEL
}

void setCounterValue(const size_t lineId, const int value) noexcept {
	const auto spaceVarsPtr = reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace);
	const uintptr_t counter = COUNTER_BASE_OFFSET + lineId * COUNTER_SIZE;
	*reinterpret_cast<int*>(spaceVarsPtr + counter) = value;
}

uintptr_t getCounterAddress(const size_t lineId) noexcept { return COUNTER_BASE_OFFSET + lineId * COUNTER_SIZE; }
} // namespace

int CounterService::addNumberCounter(const std::string& title, const int initialValue) {
	if (const auto slot = findAvailableCounterSlot(usedCounters)) {
		return addNumberCounterAtLine(title, initialValue, static_cast<unsigned short>(*slot));
	}
	return -1;
}

int CounterService::addNumberCounterAtLine(const std::string& title, const int initialValue,
                                           const unsigned short lineId) {
	if (!isValidLineId(lineId, usedCounters)) {
		return -1;
	}

	setupTextLabel(title, lineId);
	setCounterValue(lineId, initialValue);

	const auto counter = getCounterAddress(lineId);
	CUserDisplay::OnscnTimer.AddCounter(static_cast<int>(counter), 0, const_cast<char*>(indexToFtxText(lineId)),
	                                    lineId);

	usedCounters[lineId] = true;
	return static_cast<int>(lineId);
}

int CounterService::addBarCounter(const std::string& title, const int initialValue) {
	if (const auto slot = findAvailableCounterSlot(usedCounters)) {
		return addBarCounterAtLine(title, initialValue, static_cast<unsigned short>(*slot));
	}
	return -1;
}

int CounterService::addBarCounterAtLine(const std::string& title, const int initialValue, const unsigned short lineId) {
	if (!isValidLineId(lineId, usedCounters)) {
		return -1;
	}

	setupTextLabel(title, lineId);
	setCounterValue(lineId, initialValue);

	const auto counter = getCounterAddress(lineId);
	CUserDisplay::OnscnTimer.AddCounter(static_cast<int>(counter), 1, const_cast<char*>(indexToFtxText(lineId)),
	                                    lineId);

	usedCounters[lineId] = true;
	return static_cast<int>(lineId);
}

int CounterService::addStagesCounter(const std::string& title, const int initialValue, const unsigned short maxCount,
                                     const unsigned char colorId) {
	if (const auto slot = findAvailableCounterSlot(usedCounters)) {
		return addStagesCounterAtLine(title, initialValue, maxCount, colorId, static_cast<unsigned short>(*slot));
	}
	return -1;
}

int CounterService::addStagesCounterAtLine(const std::string& title, const int initialValue,
                                           const unsigned short maxCount, const unsigned char colorId,
                                           const unsigned short lineId) {
	if (!isValidLineId(lineId, usedCounters)) {
		return -1;
	}

	setupTextLabel(title, lineId);
	setCounterValue(lineId, initialValue);

	const auto counter = getCounterAddress(lineId);
	CUserDisplay::OnscnTimer.AddCounterCounter(counter, maxCount, const_cast<char*>(indexToFtxText(lineId)), lineId);
	CUserDisplay::OnscnTimer.SetCounterColourID(counter, colorId);

	usedCounters[lineId] = true;
	return static_cast<int>(lineId);
}

void CounterService::updateCounter(const int counterId, const int value) const {
	if (static_cast<size_t>(counterId) >= usedCounters.size() || !usedCounters[counterId]) {
		return;
	}

	setCounterValue(static_cast<size_t>(counterId), value);
}

void CounterService::clearCounter(const int counterId) {
	if (static_cast<size_t>(counterId) >= usedCounters.size() || !usedCounters[counterId]) {
		return;
	}

	const auto counter = getCounterAddress(static_cast<size_t>(counterId));
	CUserDisplay::OnscnTimer.ClearCounter(counter);
	usedCounters[counterId] = false;
}
