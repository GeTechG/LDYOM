#pragma once
#include <string>
#include <string_view>

namespace objective_utils {

/**
 * Determines if an objective type is "interrupting" - meaning it requires player interaction
 * or changes the gameplay state in a way that should break continuity with similar objectives.
 *
 * Non-interrupting objectives (environment/state changes):
 * - Weather changes
 * - Time changes
 * - Ped behavior settings
 * - Wanted level changes
 * - Money add/subtract
 * - etc.
 *
 * Interrupting objectives (player interaction/gameplay):
 * - Cutscenes
 * - Phone calls
 * - Checkpoints
 * - Kill actor/gang
 * - Enter vehicle
 * - Destroy vehicle
 * - Interact with object
 * - Countdown
 * - etc.
 */
inline bool isInterruptingObjective(std::string_view objectiveType) {
	// Non-interrupting objectives (environment/settings changes)
	if (objectiveType == "core.clock")
		return false; // Time change
	if (objectiveType == "core.ped_behaviour")
		return false; // Ped/vehicle spawn settings
	if (objectiveType == "core.money_add")
		return false; // Money addition
	if (objectiveType == "core.money_subtract")
		return false; // Money subtraction

	// All other objectives are considered interrupting
	return true;
}

/**
 * Finds the last interrupting objective before the given index.
 * Returns -1 if no interrupting objective is found.
 */
template <typename ObjectiveList>
int findLastInterruptingObjective(const ObjectiveList& objectives, int currentIndex) {
	for (int i = currentIndex - 1; i >= 0; --i) {
		if (isInterruptingObjective(objectives[i].type)) {
			return i;
		}
	}
	return -1;
}

/**
 * Finds the next interrupting objective after the given index.
 * Returns -1 if no interrupting objective is found.
 */
template <typename ObjectiveList>
int findNextInterruptingObjective(const ObjectiveList& objectives, int currentIndex) {
	for (int i = currentIndex + 1; i < static_cast<int>(objectives.size()); ++i) {
		if (isInterruptingObjective(objectives[i].type)) {
			return i;
		}
	}
	return -1;
}

} // namespace objective_utils
