#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <blip_color_utils.h>
#include <common.h>
#include <components/object.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::interact_with_object {
constexpr const char* TYPE = "core.interact_with_object";

struct Data {
	std::string objectUuid;
	int interactType = 0;
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveInteractWithObjectData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, objectUuid, interactType, text, textTime, colorBlip);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, objectUuid, interactType, text, textTime, colorBlip);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	renderEntityByComponentSelection<components::Object>(data.objectUuid, ImGui::GetContentRegionAvail().x, 0.45f);

	ImGui::Text(tr("interact_type").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##interact_type", tr(fmt::format("interact_types.{}", data.interactType)).c_str())) {
		for (int i = 0; i < 4; ++i) {
			if (ImGui::Selectable(tr(fmt::format("interact_types.{}", i)).c_str(), data.interactType == i)) {
				data.interactType = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputText("##text", &data.text);

	ImGui::Text(_("text_time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##text_time", &data.textTime, 0.1f, 0.f, 10.f, "%.1f");

	ImGui::Text(_("color_blip").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##color_blip", _(fmt::format("colors_blip.{}", data.colorBlip)).c_str())) {
		for (int i = 0; i < 7; ++i) {
			if (ImGui::Selectable(_(fmt::format("colors_blip.{}", i)).c_str(), data.colorBlip == i)) {
				data.colorBlip = i;
			}
		}
		ImGui::EndCombo();
	}
}

ktwait execute(Data& data) {
	std::shared_ptr<components::Object> targetObject;
	auto entities = ProjectPlayer::instance().getEntities();
	auto objectUuid = uuids::uuid::from_string(data.objectUuid);
	for (auto& entity : entities) {
		auto object = components::Object::cast(entity->getComponent(components::Object::TYPE));
		if (!object)
			continue;
		if (entity->id == objectUuid && object->handle) {
			targetObject = object;
		}
	}

	if (!targetObject) {
		co_return;
	}

	int blip = -1;
	if (data.colorBlip > 0) {
		blip = addBlipToObject(targetObject->getObjectRef(), data.colorBlip);
	}

	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.textTime * 1000.0f),
		                      0, false);
	}

	while (true) {
		auto isInteracted = false;
		switch (data.interactType) {
			case 0: // Touch
				isInteracted = plugin::Command<plugin::Commands::IS_CHAR_TOUCHING_OBJECT>(
					CPools::GetPedRef(FindPlayerPed()), targetObject->getObjectRef());
				break;
			case 1: // Damage
				isInteracted = plugin::Command<plugin::Commands::HAS_OBJECT_BEEN_DAMAGED>(targetObject->getObjectRef());
				break;
			case 2: // Photograph
				isInteracted =
					plugin::Command<plugin::Commands::HAS_OBJECT_BEEN_PHOTOGRAPHED>(targetObject->getObjectRef());
				break;
			case 3: // Shoot
				isInteracted =
					plugin::Command<plugin::Commands::HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON>(targetObject->getObjectRef());
				break;
			default: break;
		}
		if (isInteracted) {
			break;
		}
		co_await 1;
	}

	if (blip != -1) {
		plugin::Command<plugin::Commands::REMOVE_BLIP>(blip);
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "object", .builder = make}; }
} // namespace objectives::interact_with_object