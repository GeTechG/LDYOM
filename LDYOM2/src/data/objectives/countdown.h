#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <blip_color_utils.h>
#include <common.h>
#include <components/vehicle.h>
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

namespace objectives::countdown {
constexpr const char* TYPE = "core.countdown";

struct Data {
	std::string text;
	int time = 3;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveKillActorGangData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, text, time);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, text, time);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top

	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputText("##text", &data.text);

	ImGui::Text(_("time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragInt("##time", &data.time, 0.1f, 0, 10000);
}

ktwait execute(Data& data) {
	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.time * 1000.0f), 0,
		                      false);
	}

	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
	for (int i = data.time; i >= 0; --i) {
		if (i > 0) {
			plugin::Command<plugin::Commands::PRINT_WITH_NUMBER_BIG>("NUMBER", i, 900, 4);
			plugin::Command<plugin::Commands::REPORT_MISSION_AUDIO_EVENT_AT_POSITION>(0.0, 0.0, 0.0, 1056);
		} else {
			plugin::Command<plugin::Commands::PRINT_BIG>("GO", 900, 4);
			plugin::Command<plugin::Commands::REPORT_MISSION_AUDIO_EVENT_AT_POSITION>(0.0, 0.0, 0.0, 1057);
		}
		co_await 1000;
	}
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::countdown