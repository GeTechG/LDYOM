#include "plugin.h"
#include <CGame.h>

#include "core/application.h"

class LDYOM {
  private:
	bool m_initialized = false;

  public:
	LDYOM() {
		plugin::Events::shutdownRwEvent += []() {
			if (!gLDYOM.m_initialized) {
				return;
			}
			Application::instance().shutdown();
			gLDYOM.m_initialized = false;
		};
		plugin::Events::gameProcessEvent += []() {
			if (!gLDYOM.m_initialized) {
				return;
			}
			Application::instance().process();
		};
		plugin::Events::initGameEvent += []() {
			if (CGame::bMissionPackGame != 7) {
				return;
			}
			Application::instance().initialize();
			gLDYOM.m_initialized = true;
		};
	}
} gLDYOM;
