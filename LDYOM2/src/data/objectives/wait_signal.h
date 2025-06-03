#pragma once
#include "objective.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>

namespace objectives::wait_signal {
constexpr const char* TYPE = "core.wait_signal";

struct Data {
	std::string signal = "default";
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveWaitSignalData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, signal)
	}
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
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::wait_signal