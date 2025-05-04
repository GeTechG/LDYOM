#pragma once
#include "../objective.h"
#include <imgui.h>
#include <localization.h>

constexpr const char* TEST_OBJECTIVE_TYPE = "core.test";

struct TestObjectiveData {
	int value = 0;
	std::string value2 = "default";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TestObjectiveData, value, value2)
};

void renderTestObjectiveEditor(TestObjectiveData& data) {
	ImGui::InputInt("Value", &data.value);
	ImGui::Text("Value2: %s", data.value2.c_str());
}

std::function<void(TestObjectiveData&)> getEditorFunction() { return renderTestObjectiveEditor; }

Objective makeTestObjective() {
	return Objective(TEST_OBJECTIVE_TYPE, _("objectives." + std::string(TEST_OBJECTIVE_TYPE) + ".name"),
	                 TestObjectiveData{}, getEditorFunction());
}

ObjectiveBuilderData createTestObjectiveBuilder() {
	return ObjectiveBuilderData{.type = TEST_OBJECTIVE_TYPE,
	                            .category = "test.test1.test2",
	                            .builder = makeTestObjective};
}