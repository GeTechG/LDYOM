#define IMGUI_DEFINE_MATH_OPERATORS
#include "main_menu.h"
#include "projects_manager.h"
#include <fmt/format.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <utils/theme_loader.h>
#include <window_manager.h>

void MainMenu::renderContent(Window* window) {
	if (ImGui::Button("Close project")) {
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