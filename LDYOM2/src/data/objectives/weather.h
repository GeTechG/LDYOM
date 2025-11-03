#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CWeather.h>
#include <blip_color_utils.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

namespace objectives::weather {
constexpr const char* TYPE = "core.weather";

struct Data {
	int weatherId = 0;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveWeatherData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, weatherId);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, weatherId);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Weather selection
	ImGui::Text(_("weather").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragInt("##weather", &data.weatherId, 0.1f, 0, 255, tr("weather_id").c_str())) {
		CWeather::ForceWeatherNow(static_cast<short>(data.weatherId));
	}
}

ktwait execute(Data& data) {
	// Force weather change immediately
	CWeather::ForceWeatherNow(static_cast<short>(data.weatherId));

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::weather
