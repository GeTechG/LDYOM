#include "init.h"
#include "window_manager.h"
#include "windows/create_new_project.h"
#include "windows/main_menu.h"
#include "windows/project_manager.h"
#include "windows/quick_settings.h"
#include <logger.h>

void initWindows() {
	WindowManager::instance().addWindow("project_manager", std::make_unique<ProjectManager>());
	WindowManager::instance().addWindow("quick_settings", std::make_unique<QuickSettings>());
	WindowManager::instance().addWindow("create_new_project", std::make_unique<CreateNewProject>());
	WindowManager::instance().addWindow("main_menu", std::make_unique<MainMenu>());

	LDYOM_INFO("Windows initialized");
}