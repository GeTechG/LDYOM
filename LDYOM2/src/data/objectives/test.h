#pragma once
#include "../objective.h"
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <objectives_manager.h>

namespace objectives::test {
constexpr const char* TYPE = "core.test";

struct Data {
	int value = 0;
	std::string value2 = "default";
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveTestData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, value, value2)
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, value, value2)
};

void renderEditor(Data& data) {
	ImGui::InputInt("Value", &data.value);
	ImGui::Text("Value2: %s", data.value2.c_str());
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor));
}

ObjectiveBuilderData builder() {
	return ObjectiveBuilderData{.type = TYPE, .category = "test.test1.test2", .builder = make};
}
} // namespace objectives::test