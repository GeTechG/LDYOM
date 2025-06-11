#include "plugin.h"
#include <CGame.h>

#include "core/application.h"
#include "imgui_hook/render_hook.h"
#include "logger.h"

class LDYOM {
	bool m_initialized = false;

  public:
	LDYOM() {
		plugin::Events::shutdownRwEvent += []() {
			ImGuiHookRemove();
			Logger::Shutdown();
			if (gLDYOM.m_initialized) {
				Application::instance().shutdown();
				gLDYOM.m_initialized = false;
			}
		};
		plugin::Events::gameProcessEvent += []() {
			if (gLDYOM.m_initialized) {
				Application::instance().process();
			}
		};
		plugin::Events::initGameEvent += []() {
			Logger::Initialize();
			ImGuiHook();
			if (CGame::bMissionPackGame == 7) {
				Application::instance().initialize();
				gLDYOM.m_initialized = true;
			}
		};
		plugin::Events::reInitGameEvent += []() {
			if (CGame::bMissionPackGame == 7) {
				Application::instance().initialize();
				gLDYOM.m_initialized = true;
			}
		};
		plugin::Events::restartGameEvent += []() {
			if (gLDYOM.m_initialized) {
				Application::instance().shutdown();
				gLDYOM.m_initialized = false;
			}
		};
	}
} gLDYOM;