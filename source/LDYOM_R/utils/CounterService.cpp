#include "CounterService.h"

#include <CTheScripts.h>
#include <CUserDisplay.h>
#include <extensions/ScriptCommands.h>

#include "strUtils.h"

using namespace plugin;

const char* indexToFtxText(const unsigned index) {
	switch (index) {
		case 0:
			return "LDCNTR0";
		case 1:
			return "LDCNTR1";
		case 2:
			return "LDCNTR2";
		case 3:
			return "LDCNTR3";
		default:
			return "";
	}
}

int CounterService::addNumberCounter(const std::string &title, const int initialValue) {
	for (unsigned i = 0; i < this->usedCounters.size(); i++) {
		if (!this->usedCounters[i]) {
			return this->addNumberCounterAtLine(title, initialValue, static_cast<unsigned short>(i));
		}
	}
	return -1;
}

int CounterService::addNumberCounterAtLine(const std::string &title, const int initialValue,
                                           const unsigned short lineId) {
	const auto spaceVarsPtr = reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace);

	if (lineId > this->usedCounters.size())
		return -1;

	if (this->usedCounters[lineId])
		return -1;

	auto cp1251Text = utf8ToCp1251(title);
	gxtEncode(cp1251Text);
	Command<0x0ADF>(indexToFtxText(lineId), cp1251Text.c_str());

	const uintptr_t counter = 12644 + lineId * 4;

	auto &address = *reinterpret_cast<int*>(spaceVarsPtr + counter);
	address = initialValue;
	CUserDisplay::OnscnTimer.AddCounter(static_cast<int>(counter), 0,
	                                    const_cast<char*>(indexToFtxText(lineId)), lineId);

	this->usedCounters[lineId] = true;

	return lineId;
}

int CounterService::addBarCounter(const std::string &title, const int initialValue) {
	for (unsigned i = 0; i < this->usedCounters.size(); i++) {
		if (!this->usedCounters[i]) {
			return this->addBarCounterAtLine(title, initialValue, static_cast<unsigned short>(i));
		}
	}
	return -1;
}

int CounterService::addBarCounterAtLine(const std::string &title, const int initialValue,
                                        const unsigned short lineId) {
	const auto spaceVarsPtr = reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace);

	if (lineId > this->usedCounters.size())
		return -1;

	if (this->usedCounters[lineId])
		return -1;

	auto cp1251Text = utf8ToCp1251(title);
	gxtEncode(cp1251Text);
	Command<0x0ADF>(indexToFtxText(lineId), cp1251Text.c_str());

	const uintptr_t counter = 12644 + lineId * 4;

	auto &address = *reinterpret_cast<int*>(spaceVarsPtr + counter);
	address = initialValue;
	CUserDisplay::OnscnTimer.AddCounter(static_cast<int>(counter), 1,
	                                    const_cast<char*>(indexToFtxText(lineId)), lineId);

	this->usedCounters[lineId] = true;

	return lineId;
}

int CounterService::addStagesCounter(const std::string &title, const int initialValue,
                                     const unsigned short maxCount, const unsigned char colorId) {
	for (unsigned i = 0; i < this->usedCounters.size(); i++) {
		if (!this->usedCounters[i]) {
			return this->addStagesCounterAtLine(title, initialValue, maxCount, colorId, static_cast<unsigned short>(i));
		}
	}
	return -1;
}

int CounterService::addStagesCounterAtLine(const std::string &title, const int initialValue,
                                           const unsigned short maxCount,
                                           const unsigned char colorId, const unsigned short lineId) {
	const auto spaceVarsPtr = reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace);

	if (lineId > this->usedCounters.size())
		return -1;

	if (this->usedCounters[lineId])
		return -1;

	auto cp1251Text = utf8ToCp1251(title);
	gxtEncode(cp1251Text);
	Command<0x0ADF>(indexToFtxText(lineId), cp1251Text.c_str());

	const uintptr_t counter = 12644 + lineId * 4;

	auto &address = *reinterpret_cast<int*>(spaceVarsPtr + counter);
	address = initialValue;
	CUserDisplay::OnscnTimer.AddCounterCounter(counter, maxCount,
	                                           const_cast<char*>(indexToFtxText(lineId)), lineId);
	CUserDisplay::OnscnTimer.SetCounterColourID(counter, colorId);

	this->usedCounters[lineId] = true;

	return lineId;
}

void CounterService::updateCounter(const int counterId, const int value) const {
	if (static_cast<unsigned>(counterId) > this->usedCounters.size())
		return;

	if (!this->usedCounters[counterId])
		return;

	const uintptr_t counter = 12644 + counterId * 4;

	auto &address = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(CTheScripts::ScriptSpace) + counter);
	address = value;
}

void CounterService::clearCounter(const int counterId) {
	if (static_cast<unsigned>(counterId) > this->usedCounters.size())
		return;

	if (!this->usedCounters[counterId])
		return;

	const uintptr_t counter = 12644 + counterId * 4;

	CUserDisplay::OnscnTimer.ClearCounter(counter);
	this->usedCounters[counterId] = false;
}
