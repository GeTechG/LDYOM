#include "plugin.h"
#include <CGame.h>

#include "core/application.h"


class LDYOM {
public:
	LDYOM() {
		plugin::Events::shutdownRwEvent += []() {
			Application::Instance().Shutdown();
		};
		plugin::Events::initGameEvent += []() {
			if (CGame::bMissionPackGame != 7) {
				return;
			}
			Application::Instance().Initialize();
		};
	}
} gLDYOM;
