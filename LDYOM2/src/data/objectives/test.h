#pragma once
#include "../objective.h"
#include <imgui.h>
#include <localization.h>
#include <objectives_manager.h>

namespace test_objective {
constexpr const char* TYPE = "core.test";

struct Data {
	int value = 0;
	std::string value2 = "default";
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
} // namespace test_objective