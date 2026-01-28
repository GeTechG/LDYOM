#include "plugin.h"
#include <CGame.h>

#include "core/application.h"
#include "imgui_hook/imgui_hook.h"
#include "imgui_hook/render_hook.h"
#include "logger.h"
#include "utils/vehicle_render_events.h"

// Глобальные переменные для retry механизма DirectX hook
static int g_hookRetryAttempts = 0;
static constexpr int MAX_HOOK_RETRY_ATTEMPTS = 300; // ~300 * 60 кадров = ~10-15 секунд
static int g_hookRetryFrameDelay = 0;
static constexpr int HOOK_RETRY_FRAME_INTERVAL = 60; // Повтор каждые 60 кадров (~1 сек)

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
			// Retry механизм для DirectX hook - проверяем каждые 60 кадров пока не инициализируется
			if (!ImguiHook::m_bInitialized && g_hookRetryAttempts < MAX_HOOK_RETRY_ATTEMPTS) {
				g_hookRetryFrameDelay++;

				if (g_hookRetryFrameDelay >= HOOK_RETRY_FRAME_INTERVAL) {
					TryImGuiHook();

					if (!ImguiHook::m_bInitialized) {
						g_hookRetryAttempts++;
						g_hookRetryFrameDelay = 0;

						if (g_hookRetryAttempts >= MAX_HOOK_RETRY_ATTEMPTS) {
							LDYOM_ERROR("Failed to initialize ImGui after {} retry attempts", MAX_HOOK_RETRY_ATTEMPTS);
						}
					}
				}
			}

			if (gLDYOM.m_initialized) {
				Application::instance().process();
			}
		};
		plugin::Events::initGameEvent += []() {
			Logger::Initialize();
			TryImGuiHook();
			VehicleRenderEvents::initializeEvents();

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