#include "init.h"
#include "window_manager.h"
#include "windows/addons_settings.h"
#include "windows/create_new_component.h"
#include "windows/create_new_objective.h"
#include "windows/create_new_project.h"
#include "windows/entities.h"
#include "windows/entities_menu.h"
#include "windows/main_menu.h"
#include "windows/objectives.h"
#include "windows/project_manager.h"
#include "windows/quick_settings.h"
#include "windows/scene_settings.h"
#include "windows/scenes.h"
#include <logger.h>


void initWindows() {
	WindowManager::instance().addWindow("project_manager", std::make_unique<ProjectManager>());
	WindowManager::instance().addWindow("quick_settings", std::make_unique<QuickSettings>());
	WindowManager::instance().addWindow("create_new_project", std::make_unique<CreateNewProject>());
	WindowManager::instance().addWindow("main_menu", std::make_unique<MainMenu>());
	WindowManager::instance().addWindow("addons_settings", std::make_unique<AddonsSettings>());
	WindowManager::instance().addWindow("scenes", std::make_unique<ScenesWindow>());
	WindowManager::instance().addWindow("objectives", std::make_unique<ObjectivesWindow>());
	WindowManager::instance().addWindow("create_new_objective", std::make_unique<CreateNewObjective>());
	WindowManager::instance().addWindow("create_new_component", std::make_unique<CreateNewComponent>());
	WindowManager::instance().addWindow("entities_menu", std::make_unique<EntitiesMenu>());
	WindowManager::instance().addWindow("entities", std::make_unique<EntitiesWindow>());
	WindowManager::instance().addWindow("scene_settings", std::make_unique<SceneSettingsPopup>());

	LDYOM_INFO("Windows initialized");
}