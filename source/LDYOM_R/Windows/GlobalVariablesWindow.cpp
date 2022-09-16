#include "GlobalVariablesWindow.h"

#include "constants.h"
#include "fa.h"
#include "imgui.h"
#include "LuaEngine.h"
#include "Localization/Localization.h"
#include "fmt/core.h"
#include "NodeEditorUtils.h"

void Windows::GlobalVariablesWindow::draw() {
	static auto& local = Localization::getInstance();
	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Once, ImVec2(.5f, .5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 15.f, ImGui::GetFontSize() * 10.f), ImGuiCond_Once);

	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_BOX, local.get("general.global_variables")).c_str(), nullptr, ImGuiWindowFlags_NoCollapse)) {
		auto globalData = LuaEngine::getInstance().getLuaState()["global_data"];
		if (ImGui::BeginChild("##variables", ImVec2(0.f, -ImGui::GetFontSize() * 2.f))) {
			auto variables = globalData["variables"].get_or_create<sol::table>();
			for (auto pair : variables) {
				auto variable = pair.second.as<sol::table>();

				auto removeNodes = [&]() {
					for (auto context : globalData["ed_contexts"].get<sol::table>()) {
						context.second.as<sol::table>()["deleteNodeByClass"](context.second, globalData["GetVariableNode"], "varId", pair.first);
						context.second.as<sol::table>()["deleteNodeByClass"](context.second, globalData["SetVariableNode"], "varId", pair.first);
					}
				};

				ImGui::PushID(pair.first.as<int>());
				ImGui::PushItemWidth(ImGui::GetFontSize() * 6.f);
				ImGui::InputTextWithHint("##namVar", local.get("general.name").c_str(), static_cast<char*>(const_cast<void*>(variable["name"].get<sol::object>().pointer())), TEXT_SIZE);
				ImGui::SameLine();
				int type = variable["type"].get<int>();
				if (ImGui::SliderInt("##type", &type, 1, 3, toString(static_cast<ValueType>(type)))) {
					variable["setType"](variable, type);
					removeNodes();
				}
				ImGui::SameLine();
				switch (static_cast<ValueType>(type)) {
					case Number:
						ImGui::InputFloat("##varValue", static_cast<float*>(const_cast<void*>(variable["value"].get<sol::object>().pointer())));
						break;
					case Boolean:
						ImGui::Checkbox("##varValue", static_cast<bool*>(const_cast<void*>(variable["value"].get<sol::object>().pointer())));
						break;
					case String:
						ImGui::InputText("##varValue", static_cast<char*>(const_cast<void*>(variable["value"].get<sol::object>().pointer())), TEXT_SIZE);
						break;
					default:
						break;
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_TIMES)) {
					variables[pair.first] = sol::nil;
					removeNodes();
				}
				ImGui::PopID();
			}
		}
		ImGui::EndChild();
		if (ImGui::Button(local.get("general.add").c_str())) {
			globalData["variables"].get_or_create<sol::table>().add(globalData["Variable"]["new"](globalData["Variable"]));
		}
	}
	ImGui::End();
}
