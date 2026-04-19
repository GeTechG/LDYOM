#pragma once
#include "objective.h"
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <project_player.h>
#include <scenes_manager.h>

namespace objectives::goto_scene {
constexpr const char* TYPE = "core.goto_scene";

struct Data {
	std::string sceneId;
	bool instantTransition = false;
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveGotoSceneData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, sceneId, instantTransition)
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, sceneId, instantTransition)
};

void renderEditor(Data& data) {
	const auto& scenes = ScenesManager::instance().getScenesInfo();
	std::string preview;
	for (const auto& scene : scenes) {
		if (scene.id == data.sceneId) {
			preview = scene.name;
			break;
		}
	}
	if (ImGui::BeginCombo(_("objectives.core.goto_scene.scene").c_str(), preview.c_str())) {
		for (const auto& scene : scenes) {
			bool selected = scene.id == data.sceneId;
			if (ImGui::Selectable(scene.name.c_str(), selected)) {
				data.sceneId = scene.id;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Checkbox(_("objectives.core.goto_scene.instant_transition").c_str(), &data.instantTransition);
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("%s", _("objectives.core.goto_scene.instant_transition_tooltip").c_str());
	}
}

ktwait execute(Data& data) {
	ProjectPlayer::instance().requestSceneTransition(data.sceneId, data.instantTransition);
	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::goto_scene
