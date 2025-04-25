#include "application.h"
#include "configuration.h"
#include "hotkeys.h"
#include "localization.h"
#include "render_hook.h"
#include <logger.h>
#include <plugin.h>
#include <projects_manager.h>
#include <sol/sol.hpp>
#include <utils/theme_loader.h>
#include <window_manager.h>
#include <windows/init.h>

void Application::Initialize() {
	Logger::Initialize();
	LDYOM_INFO("LDYOM Application starting...");

	try {
		Configuration::instance().initialize();
		Localization::instance().initialize();
		Hotkeys::instance().initialize();
		WindowManager::instance().initialize();
		ThemeLoader::initialize();
		ProjectsManager::instance().initialize();

		HookImgui(RenderFrames);

		Hotkeys::instance().addHotkeyCallback("openEditor", []() {
			const auto currentProjectIndex = ProjectsManager::instance().getCurrentProjectIndex();
			if (currentProjectIndex == -1) {
				if (WindowManager::instance().isWindowOpen("project_manager")) {
					WindowManager::instance().closeWindow("project_manager");
				} else {
					WindowManager::instance().openWindow("project_manager");
				}
			} else {
				if (WindowManager::instance().isWindowOpen("main_menu")) {
					WindowManager::instance().closeWindow("main_menu");
				} else {
					WindowManager::instance().openWindow("main_menu");
				}
			}
		});

		initWindows();

		// WindowManager::instance().registerHotkey(ImGuiKey_I, false, false, false, "project_manager");
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

void Application::Shutdown() {
	LDYOM_INFO("Application shutdown");
	WindowManager::instance().shutdown();
	LDYOM_INFO("Window Manager shutdown");

	UnhookImgui();
	LDYOM_INFO("ImGui unhooked");

	Localization::instance().shutdown();

	Logger::Shutdown();
}

void Application::RenderFrames() {
	Hotkeys::instance().update();
	rocket::dispatch_queued_calls();
	WindowManager::instance().render();
}