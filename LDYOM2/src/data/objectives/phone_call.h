#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_widgets/imgui_widgets.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils/objective_utils.h>

namespace objectives::phone_call {
constexpr const char* TYPE = "core.phone_call";

struct Data {
	std::string text;
	float duration = 5.0f; // Duration in seconds

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePhoneCallData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, text, duration);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, text, duration);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top

	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputTextWithPopup("##text", &data.text);

	ImGui::Text(tr("duration").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##duration", &data.duration, 0.01f, 0.0f, 120.0f, "%.2f s");
}

ktwait execute(Data& data) {
	auto currentObjectiveIndex = ProjectPlayer::instance().getCurrentObjectiveIndex();
	auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;

	// Check if last interrupting objective was also a phone call
	bool shouldStartCall = !objective_utils::isLastInterruptingObjectiveOfType(objectives, currentObjectiveIndex, TYPE);

	// Check if next interrupting objective is also a phone call
	bool shouldEndCall = !objective_utils::isNextInterruptingObjectiveOfType(objectives, currentObjectiveIndex, TYPE);

	constexpr int CELLPHONE_MODEL = 330; // Model ID for cellphone

	// Start phone call only if not already active
	if (shouldStartCall) {
		plugin::Command<plugin::Commands::REQUEST_MODEL>(CELLPHONE_MODEL);

		while (!plugin::Command<plugin::Commands::HAS_MODEL_LOADED>(CELLPHONE_MODEL)) {
			co_await 10;
		}

		// Make player hold cellphone (opcode 0729)
		plugin::Command<plugin::Commands::TASK_USE_MOBILE_PHONE>(CPools::GetPedRef(FindPlayerPed()), true);

		// Wait for animation to start
		co_await 2000;
	}

	// Display text if provided
	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), 5000, 0, false);
	}

	// Wait for the specified duration (in milliseconds)
	int durationMs = static_cast<int>(data.duration * 1000.0f);
	co_await durationMs;

	// End phone call only if next objective is not a phone call
	if (shouldEndCall) {
		plugin::Command<plugin::Commands::TASK_USE_MOBILE_PHONE>(CPools::GetPedRef(FindPlayerPed()), false);
		plugin::Command<plugin::Commands::MARK_MODEL_AS_NO_LONGER_NEEDED>(CELLPHONE_MODEL);
		co_await 2000; // Wait for animation to end
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::phone_call
