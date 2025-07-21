#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <blip_color_utils.h>
#include <components/actor.h>
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

namespace objectives::kill_actor_gang {
constexpr const char* TYPE = "core.kill_actor_gang";

struct Data {
	std::string actorUuid;
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;
	bool killWholeGang = false;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveKillActorGangData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, actorUuid, text, textTime, colorBlip, killWholeGang);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, actorUuid, text, textTime, colorBlip, killWholeGang)
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	renderEntityByComponentSelection<components::Actor>(data.actorUuid, ImGui::GetContentRegionAvail().x, 0.45f);
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

	ImGui::Text(tr("kill_whole_gang").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##kill_whole_gang", &data.killWholeGang);
}

ktwait execute(Data& data) {
	std::shared_ptr<components::Actor> targetActor;
	auto entities = ProjectPlayer::instance().getEntities();
	auto actorUuid = uuids::uuid::from_string(data.actorUuid);
	for (auto& entity : entities) {
		auto actor = components::Actor::cast(entity->getComponent(components::Actor::TYPE));
		if (!actor)
			continue;
		if (entity->id == actorUuid && actor->ped) {
			targetActor = actor;
		}
	}
	std::vector<int> actorsToKill;
	if (targetActor) {
		if (data.killWholeGang) {
			for (auto& entity : entities) {
				auto actor = components::Actor::cast(entity->getComponent(components::Actor::TYPE));
				if (!actor)
					continue;
				if (actor->ped) {
					auto eqSimpleType = actor->isSimpleType == targetActor->isSimpleType;
					auto eqPedType = actor->pedType == targetActor->pedType;
					if (eqSimpleType && eqPedType) {
						actorsToKill.push_back(actor->getPedRef());
					}
				}
			}
		} else {
			actorsToKill.push_back(targetActor->getPedRef());
		}
	}

	std::vector<int> blips;
	if (data.colorBlip > 0) {
		blips.reserve(actorsToKill.size());
		for (auto& actor : actorsToKill) {
			blips.push_back(addBlipToPed(actor, data.colorBlip));
		}
	}

	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.textTime * 1000.0f),
		                      0, false);
	}

	while (!actorsToKill.empty()) {
		auto actor = actorsToKill.front();
		if (!plugin::Command<plugin::Commands::DOES_CHAR_EXIST>(actor) ||
		    plugin::Command<plugin::Commands::IS_CHAR_DEAD>(actor)) {
			actorsToKill.erase(actorsToKill.begin());
			if (!blips.empty()) {
				plugin::Command<plugin::Commands::REMOVE_BLIP>(blips.front());
				blips.erase(blips.begin());
			}
			continue;
		}
		co_await 10;
	}
	for (auto& blip : blips) {
		plugin::Command<plugin::Commands::REMOVE_BLIP>(blip);
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "actor", .builder = make}; }
} // namespace objectives::kill_actor_gang