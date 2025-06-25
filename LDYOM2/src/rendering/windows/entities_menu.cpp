#define IMGUI_DEFINE_MATH_OPERATORS
#include "entities_menu.h"
#include "entities.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

void EntitiesMenu::renderContent(Window* window) {
	// Calculate the maximum button width needed
	float buttonWidth = 250.0f * (SCL_PX).x;
	ImVec2 buttonSize(buttonWidth, 0.0f); // Height 0 means use default height

	auto openEntitiesWindow = [window](EntitiesWindowType type) {
		auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
		if (entitiesWindow) {
			entitiesWindow.value()->setWindowType(type);
		}
		WindowManager::instance().openWindow("entities");
		window->close();
	};

	if (ImGui::Button(_("entities_menu.actors", ICON_FA_PERSON).c_str(), buttonSize)) {
		openEntitiesWindow(EntitiesWindowType_Actor);
	}
	if (ImGui::Button(_("entities_menu.vehicles", ICON_FA_STEERING_WHEEL).c_str(), buttonSize)) {
		openEntitiesWindow(EntitiesWindowType_Vehicle);
	}
	if (ImGui::Button(_("entities_menu.objects", ICON_FA_CUBES).c_str(), buttonSize)) {
		openEntitiesWindow(EntitiesWindowType_Object);
	}
	if (ImGui::Button(_("entities_menu.pickups", ICON_FA_HEART).c_str(), buttonSize)) {
		openEntitiesWindow(EntitiesWindowType_Pickup);
	}
}

EntitiesMenu::EntitiesMenu()
	: Window(_("entities_menu.title", ICON_FA_USER_ALIEN)) {
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_AlwaysAutoResize);
	setRenderCallback(EntitiesMenu::renderContent);
}