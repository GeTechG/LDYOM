#include "init.h"
#include "scenes_settings.h"
#include "window_manager.h"
#include "windows/addons_settings.h"
#include "windows/create_new_objective.h"
#include "windows/create_new_project.h"
#include "windows/main_menu.h"
#include "windows/objectives.h"
#include "windows/project_manager.h"
#include "windows/quick_settings.h"
#include <logger.h>


void initWindows() {
	WindowManager::instance().addWindow("project_manager", std::make_unique<ProjectManager>());
	WindowManager::instance().addWindow("quick_settings", std::make_unique<QuickSettings>());
	WindowManager::instance().addWindow("create_new_project", std::make_unique<CreateNewProject>());
	WindowManager::instance().addWindow("main_menu", std::make_unique<MainMenu>());
	WindowManager::instance().addWindow("addons_settings", std::make_unique<AddonsSettings>());
	WindowManager::instance().addWindow("scenes_settings", std::make_unique<ScenesSettings>());
	WindowManager::instance().addWindow("objectives", std::make_unique<ObjectivesWindow>());
	WindowManager::instance().addWindow("create_new_objective", std::make_unique<CreateNewObjective>());

	LDYOM_INFO("Windows initialized");
}