#include "CutsceneMutex.h"

bool CutsceneMutex::isLocked() {
	return lock_;
}
