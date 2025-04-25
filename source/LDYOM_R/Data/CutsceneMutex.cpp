#include "CutsceneMutex.h"

void CutsceneMutex::free() {
	lock_ = false;
}

bool CutsceneMutex::isLocked() {
	return lock_;
}
