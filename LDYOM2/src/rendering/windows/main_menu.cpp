#define IMGUI_DEFINE_MATH_OPERATORS
#include "main_menu.h"
#include "projects_manager.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <localization.h>
#include <project_player.h>
#include <utils/imgui_configurate.h>
#include <utils/theme_loader.h>
#include <window_manager.h>

void MainMenu::renderContent(Window* window) {
	// Calculate the maximum button width needed
	float buttonWidth = 250.0f * (SCL_PX).x;
	ImVec2 buttonSize(buttonWidth, 0.0f); // Height 0 means use default height

	if (ImGui::Button(_("entities_menu.title", ICON_FA_USER_ALIEN).c_str(), buttonSize)) {
		WindowManager::instance().openWindow("entities_menu");
		window->close();
	}
	if (ImGui::Button(_("objectives.title", ICON_FA_BULLSEYE_ARROW).c_str(), buttonSize)) {
		WindowManager::instance().openWindow("objectives");
		window->close();
	}
	if (ImGui::Button(_("main_menu.run_project", ICON_FA_PLAY).c_str(), buttonSize)) {
		ProjectPlayer::instance().startCurrentProject();
		WindowManager::instance().closeAllWindows();
	}
	if (ImGui::Button(_("scenes.title", ICON_FA_FILM).c_str(), buttonSize)) {
		WindowManager::instance().openWindow("scenes");
		window->close();
	}
	if (ImGui::Button(_("scene_settings.title", ICON_FA_SLIDERS).c_str(), buttonSize)) {
		WindowManager::instance().openWindow("scene_settings");
	}
	if (ImGui::Button(_("addons_settings.title", ICON_FA_PUZZLE_PIECE).c_str(), buttonSize)) {
		WindowManager::instance().openWindow("addons_settings");
	}
	if (ImGui::Button(_("main_menu.close_project", ICON_FA_RIGHT_FROM_BRACKET).c_str(), buttonSize)) {
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