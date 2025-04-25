#include "pad.h"
#include "logger.h"
#include <plugin.h>
#include <unordered_set>

std::unordered_set<int> lockControls;

void updateLockControls() {
	FindPlayerPed()->GetPadFromPlayer()->DisablePlayerControls = !lockControls.empty();
}

void addLockControl(int id) {
	lockControls.insert(id);
	updateLockControls();
}

void removeLockControl(int id) {
	lockControls.erase(id);
	updateLockControls();
}