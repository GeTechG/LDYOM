#define IMGUI_DEFINE_MATH_OPERATORS
#include "main_menu.h"
#include "projects_manager.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <utils/theme_loader.h>
#include <window_manager.h>

void MainMenu::renderContent(Window* window) {
	if (ImGui::Button(_("scenes_settings.title", ICON_FA_FILM).c_str())) {
		WindowManager::instance().openWindow("scenes_settings");
		window->close();
	}
	if (ImGui::Button(_("addons_settings.title", ICON_FA_PUZZLE_PIECE).c_str())) {
		WindowManager::instance().openWindow("addons_settings");
	}
	if (ImGui::Button(_("main_menu.close_project", ICON_FA_RIGHT_FROM_BRACKET).c_str())) {
		ProjectsManager::instance().closeProject();
		window->close();
		WindowManager::instance().openWindow("project_manager");
	}
}

MainMenu::MainMenu()
	: Window("main_menu.title") {
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_AlwaysAutoResize);
	setRenderCallback(MainMenu::renderContent);
}