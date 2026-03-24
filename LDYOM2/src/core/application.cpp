#include "application.h"
#include "addons_manager.h"
#include "entities_manager.h"
#include "hotkeys.h"
#include "imgui_hook.h"
#include "localization.h"
#include "lua_manager.h"
#include "models_manager.h"
#include "object_tags_manager.h"
#include "objectives_manager.h"
#include "project_player.h"
#include "project_script_manager.h"
#include "settings.h"
#include "textures_manager.h"
#include <components_manager.h>
#include <fires_remover.h>
#include <in_game/debug_info.h>
#include <nothing.h>
#include <plugin.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <sol/sol.hpp>
#include <task_manager.h>
#include <utils/teleport_utils.h>
#include <utils/theme_loader.h>
#include <window_manager.h>
#include <windows/init.h>
#include <windows/node_editor.h>


void Application::initialize() {
	LDYOM_INFO("LDYOM Application starting...");

	try {
		Settings::instance().initialize();
		Localization::instance().initialize();
		ModelsManager::loadData();
		ObjectTagsManager::instance().initialize();
		Hotkeys::instance().initialize();
		TexturesManager::instance().initialize();
		WindowManager::instance().initialize();
		ThemeLoader::initialize();
		ProjectsManager::instance().initialize();
		LuaManager::instance().initialize();
		ProjectScriptManager::instance().initialize();
		AddonsManager::instance().initialize();

		ImguiHook::setRenderFunc(renderFrames);
		ImguiHook::dirtyObjectsFlag();

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

		Hotkeys::instance().addHotkeyCallback("toggleDebugInfo", []() { DebugInfo::toggle(); });

		Hotkeys::instance().addHotkeyCallback("teleportToMarker", []() {
			if (!WindowManager::instance().isAnyWindowOpen()) {
				TaskManager::instance().addTask("teleportToMarker", teleportPlayerToMarker);
			}
		});

		Hotkeys::instance().addHotkeyCallback("unlockPlayer", []() {
			if (!WindowManager::instance().isAnyWindowOpen()) {
				unlockPlayer();
			}
		});

		initWindows();

		// Register `node_tasks` Lua table bound to the node editor's own LuaTaskManager.
		// Must happen after initWindows() so the NodeEditorWindow exists.
		if (auto nodeEditor = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		    nodeEditor && *nodeEditor) {
			auto guard = LuaManager::instance().getState();
			LuaTaskManager::registerBindings(guard.get(), (*nodeEditor)->taskManager(), "node_tasks");
		}

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

	ImguiHook::setRenderFunc(nullptr);
	ImguiHook::SetMouseState(false);
	ImguiHook::SetControlEnabled(true);
	LDYOM_INFO("ImGui Hook render function cleared");

	Localization::instance().shutdown();
	Hotkeys::instance().shutdown();
	m_luaTaskManager.shutdown();
	ProjectScriptManager::instance().unloadProjectScript();
	LuaManager::instance().shutdown();
	TaskManager::instance().shutdown();
	ScenesManager::instance().setRestartGame(true);
	ProjectsManager::instance().shutdown();
	ScenesManager::instance().setRestartGame(false);
}

void Application::process() {
	rocket::dispatch_queued_calls();

	// Clear pin recursion blacklist once per game frame so node execution outside UI works correctly
	if (auto nodeEditor = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor")) {
		if (*nodeEditor) {
			if (auto* graph = (*nodeEditor)->getNodeFlow())
				graph->clear_recursion_blacklist();
			(*nodeEditor)->taskManager().processAll();
		}
	}

	nothing();

	ScenesManager::instance().onUpdate((CTimer::m_snTimeInMilliseconds - CTimer::m_snPreviousTimeInMilliseconds) /
	                                   1000.f);
	TaskManager::instance().processAll();
	m_luaTaskManager.processAll();

	// Remove game fires that are not associated with any pyrotechnic
	checkEditorFires();
}

void Application::renderFrames() {
	Hotkeys::instance().update();
	rocket::dispatch_queued_calls();
	WindowManager::instance().render();
}