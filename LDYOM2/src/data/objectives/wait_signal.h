#pragma once
#include "objective.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <project_player.h>

namespace wait_signal_objective {
constexpr const char* TYPE = "core.wait_signal";

struct Data {
	std::string signal = "default";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, signal)
};

void renderEditor(Data& data) { ImGui::InputText(_("signal").c_str(), &data.signal); }

ktwait execute(Data& data) {
	bool isReceived = false;
	rocket::scoped_connection connection = ProjectPlayer::instance().signals.connect(
		[&data, &isReceived](std::string signal) {
			if (signal == data.signal) {
				isReceived = true;
			}
		},
		rocket::connection_flags::queued_connection);
	while (!isReceived) {
		co_await 1;
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace wait_signal_objective