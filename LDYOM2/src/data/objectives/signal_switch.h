#pragma once
#include "objective.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <uuid_wrap.h>

namespace objectives::signal_switch {
constexpr const char* TYPE = "core.signal_switch";

struct SignalEntry {
	std::string signal;
	std::string objectiveId; // UUID string of target objective

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(SignalEntry, signal, objectiveId)
};

struct Data {
	std::vector<SignalEntry> entries;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveSignalSwitchData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, entries);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, entries)
};

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 400.f, 0.f));

	const auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

	auto getObjectiveName = [&](const std::string& id) -> std::string {
		for (const auto& obj : objectives) {
			if (uuids::to_string(obj.id) == id)
				return obj.name;
		}
		return "...";
	};

	ImGui::Text(_("objectives.core.signal_switch.entries").c_str());

	float maxHeight = (SCL_PX).y * 160.f;
	if (ImGui::BeginTable("##signal_switch_entries", 3,
	                      ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY,
	                      ImVec2(0.f, maxHeight))) {
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn(_("objectives.core.signal_switch.signal").c_str(), ImGuiTableColumnFlags_WidthStretch,
		                        0.45f);
		ImGui::TableSetupColumn(_("objectives.core.signal_switch.objective").c_str(),
		                        ImGuiTableColumnFlags_WidthStretch, 0.45f);
		ImGui::TableSetupColumn("##del", ImGuiTableColumnFlags_WidthFixed, (SCL_PX).x * 24.f);
		ImGui::TableHeadersRow();

		int toRemove = -1;
		for (int i = 0; i < static_cast<int>(data.entries.size()); i++) {
			ImGui::TableNextRow();
			ImGui::PushID(i);

			ImGui::TableSetColumnIndex(0);
			ImGui::SetNextItemWidth(-1.f);
			ImGui::InputText("##sig", &data.entries[i].signal);

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-1.f);
			std::string preview = getObjectiveName(data.entries[i].objectiveId);
			if (ImGui::BeginCombo("##obj", preview.c_str())) {
				for (size_t j = 0; j < objectives.size(); j++) {
					const auto& obj = objectives[j];
					std::string objIdStr = uuids::to_string(obj.id);
					bool selected = data.entries[i].objectiveId == objIdStr;
					if (ImGui::Selectable(fmt::format("{}##{}", obj.name, j).c_str(), selected))
						data.entries[i].objectiveId = objIdStr;
					if (selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::TableSetColumnIndex(2);
			if (ImGui::SmallButton("X"))
				toRemove = i;

			ImGui::PopID();
		}

		if (toRemove >= 0)
			data.entries.erase(data.entries.begin() + toRemove);

		ImGui::EndTable();
	}

	if (ImGui::Button(_("objectives.core.signal_switch.add_entry").c_str()))
		data.entries.push_back(SignalEntry{});
}

ktwait execute(Data& data) {
	std::optional<std::string> receivedSignal;
	rocket::scoped_connection connection = ProjectPlayer::instance().signals.connect(
		[&receivedSignal](std::string signal) {
			if (!receivedSignal.has_value())
				receivedSignal = signal;
		},
		rocket::connection_flags::queued_connection);

	while (!receivedSignal.has_value()) {
		co_await 1;
	}

	for (const auto& entry : data.entries) {
		if (entry.signal == receivedSignal.value() && !entry.objectiveId.empty()) {
			ProjectPlayer::instance().requestObjectiveJump(entry.objectiveId);
			break;
		}
	}
	// If no entry matched, execution continues to the next objective naturally
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::signal_switch
