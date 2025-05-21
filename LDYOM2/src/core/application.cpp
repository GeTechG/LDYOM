#include "application.h"
#include "addons_manager.h"
#include "configuration.h"
#include "entities_manager.h"
#include "hotkeys.h"
#include "localization.h"
#include "lua_manager.h"
#include "objectives_manager.h"
#include "project_player.h"
#include "render_hook.h"
#include "textures_manager.h"
#include <logger.h>
#include <plugin.h>
#include <projects_manager.h>
#include <sol/sol.hpp>
#include <task_manager.h>
#include <utils/theme_loader.h>
#include <window_manager.h>
#include <windows/init.h>

void Application::initialize() {
	Logger::Initialize();
	LDYOM_INFO("LDYOM Application starting...");

	try {
		Configuration::instance().initialize();
		Localization::instance().initialize();
		Hotkeys::instance().initialize();
		TexturesManager::instance().initialize();
		WindowManager::instance().initialize();
		ThemeLoader::initialize();
		ProjectsManager::instance().initialize();
		LuaManager::instance().initialize();
		AddonsManager::instance().initialize();

		HookImgui(renderFrames);

		Hotkeys::instance().addHotkeyCallback("openEditor", []() {
			const auto currentProjectIndex = ProjectsManager::instance().getCurrentProjectIndex();

			if (!ProjectPlayer::instance().isPlaying()) {
				if (WindowManager::instance().isAnyWindowOpen()) {
					WindowManager::instance().closeAllWindows();
				} else {
					if (currentProjectIndex == -1)
						WindowManager::instance().openWindow("project_manager");
					else
						WindowManager::instance().openWindow("main_menu");
				}
			}
		});

		initWindows();

		ObjectivesManager::instance().registerCoreObjectives();
		ComponentsManager::instance().registerCoreComponents();
		EntitiesManager::instance().registerCoreEntityTemplates();
	} catch (const std::exception& e) {
		LDYOM_CRITICAL("Error during application initialization: {}", e.what());
		LDYOM_DUMP_BACKTRACE();
		exit(1);
	} catch (...) {
		LDYOM_CRITICAL("Unknown error during application initialization");
		exit(1);
	}

	LDYOM_INFO("Application initialization completed");
}

void Application::shutdown() {
	LDYOM_INFO("Application shutdown");

	TexturesManager::instance().shutdown();
	AddonsManager::instance().shutdown();
	WindowManager::instance().shutdown();
	LDYOM_INFO("Window Manager shutdown");

	UnhookImgui();
	LDYOM_INFO("ImGui unhooked");

	Localization::instance().shutdown();
	LuaManager::instance().shutdown();
	TaskManager::instance().shutdown();

	Logger::Shutdown();
}

void Application::process() { TaskManager::instance().processAll(); }

void Application::renderFrames() {
	Hotkeys::instance().update();
	rocket::dispatch_queued_calls();
	WindowManager::instance().render();
}