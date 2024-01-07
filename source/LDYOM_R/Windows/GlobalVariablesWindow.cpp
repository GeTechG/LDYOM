#include "GlobalVariablesWindow.h"

#include "fa.h"
#include "GlobalVariablesService.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "fmt/xchar.h"
#include "Localization/Localization.h"

void Windows::GlobalVariablesWindow::draw() {
	auto &local = Localization::getInstance();
	auto &globalVariablesService = GlobalVariablesService::getInstance();

	const auto fontScale = ImGui::GetFontSize() / 16.f;

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing,
	                        ImVec2(.5f, .5f));
	if (ImGui::Begin(local.get("global_variables.title").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		auto deleteIndex = -1;
		if (ImGui::BeginChild("##global_variables_list", ImVec2(fontScale * 480.f, fontScale * 400.f), true)) {
			for (int i = 0; i < globalVariablesService.getSize(); ++i) {
				auto updateIndex = -1;

				auto [result, varView] = globalVariablesService.get(i);
				ImGui::PushID(i);
				ImGui::SetNextItemWidth(fontScale * 160.f);
				if (ImGui::InputText("##name", &varView.name)) {
					updateIndex = i;
				}
				ImGui::SameLine();

				ImGui::SetNextItemWidth(fontScale * 120.f);
				auto typesStrings = local.getArray("global_variables.types");
				if (ImGui::BeginCombo("##type", typesStrings[static_cast<int>(varView.value.getType())].c_str())) {
					for (int n = 0; n < static_cast<int>(GlobalVariableType::Bool) + 1; ++n) {
						const bool isSelected = varView.value.getType() == static_cast<GlobalVariableType>(n);
						if (ImGui::Selectable(typesStrings[n].c_str(), isSelected)) {
							varView.value.setType(static_cast<GlobalVariableType>(n));
							updateIndex = i;
						}
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();

				ImGui::SetNextItemWidth(fontScale * 160.f);
				const float leftValueX = ImGui::GetCursorPosX();
				switch (varView.value.getType()) {
					case GlobalVariableType::Float: {
						if (ImGui::InputFloat("##value", static_cast<float*>(varView.value.value)))
							updateIndex = i;
						break;
					}
					case GlobalVariableType::Int: {
						if (ImGui::InputInt("##value", static_cast<int*>(varView.value.value), 0, 0))
							updateIndex = i;
						break;
					}
					case GlobalVariableType::String: {
						if (ImGui::InputText("##value", static_cast<std::string*>(varView.value.value)))
							updateIndex = i;
						break;
					}
					case GlobalVariableType::Bool: {
						const auto v = static_cast<bool*>(varView.value.value);
						auto booleanStrings = local.getArray("global_variables.bool_values");
						std::string &label = *v ? booleanStrings[1] : booleanStrings[0];
						if (ImGui::Checkbox(label.c_str(), v))
							updateIndex = i;
						break;
					}
					default:
						break;
				}
				ImGui::SameLine(fontScale * 160.f - (ImGui::GetCursorPosX() - leftValueX) + fontScale * 10.f);

				ImGui::SetNextItemWidth(fontScale * 10.f);
				if (ImGui::Button(ICON_FA_TRASH)) {
					deleteIndex = i;
				}

				if (updateIndex != -1) {
					globalVariablesService.set(updateIndex, varView);
				}

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		const float windowWidth = ImGui::GetWindowWidth();

		const auto addString = local.get("general.add");
		ImGui::SetCursorPosX(windowWidth / 2 - (fontScale * 160.f) / 2 - fontScale * 10.f);
		if (ImGui::Button(addString.c_str(), ImVec2(fontScale * 160.f, 0))) {
			globalVariablesService.create(
				fmt::format("{}{}", local.get("global_variables.new_variable"), globalVariablesService.getSize()),
				GlobalVariableType::Int);
		}

		if (deleteIndex != -1) {
			globalVariablesService.remove(deleteIndex);
		}
	}
	ImGui::End();
}
