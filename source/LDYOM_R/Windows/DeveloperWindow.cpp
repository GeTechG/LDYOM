#include "DeveloperWindow.h"

#include <extensions/ScriptCommands.h>

#include "GlobalVariablesService.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectPlayerService.h"
#include "../Data/Scene.h"
#include "Localization/Localization.h"

using namespace plugin;

void Windows::DeveloperWindow::draw() {
	auto &local = Localization::getInstance();
	const auto fontScale = ImGui::GetFontSize() / 16.f;
	ImGui::SetNextWindowSize(ImVec2(500 * fontScale, 440 * fontScale), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
	if (ImGui::Begin(local.get("developer_dock.title").c_str(), &this->show_)) {
		if (ImGui::CollapsingHeader(local.get("global_variables.title").c_str())) {
			for (int i = 0; i < GlobalVariablesService::getInstance().getSize(); ++i) {
				auto [result, var] = GlobalVariablesService::getInstance().get(i);
				auto globalVariablesManager = ProjectPlayerService::getInstance().getGlobalVariablesManager();
				switch (auto varPlaying = globalVariablesManager.getGlobalVariable(var.uuid); varPlaying.getType()) {
					case GlobalVariableType::Float: {
						if (ImGui::InputFloat(var.name.c_str(), static_cast<float*>(varPlaying.value)))
							globalVariablesManager.setGlobalVariable(var.uuid, *static_cast<float*>(varPlaying.value));
						break;
					}
					case GlobalVariableType::Int: {
						if (ImGui::InputInt(var.name.c_str(), static_cast<int*>(varPlaying.value), 0, 0))
							globalVariablesManager.setGlobalVariable(var.uuid, *static_cast<int*>(varPlaying.value));
						break;
					}
					case GlobalVariableType::Bool: {
						if (ImGui::Checkbox(var.name.c_str(), static_cast<bool*>(varPlaying.value)))
							globalVariablesManager.setGlobalVariable(var.uuid, *static_cast<bool*>(varPlaying.value));
						break;
					}
					case GlobalVariableType::String: {
						if (ImGui::InputText(var.name.c_str(), static_cast<std::string*>(varPlaying.value)))
							globalVariablesManager.setGlobalVariable(
								var.uuid, *static_cast<std::string*>(varPlaying.value));
						break;
					}
				}
			}
		}
		if (ImGui::CollapsingHeader(local.get("entities.actors").c_str())) {
			if (ProjectPlayerService::getInstance().getCurrentScene().has_value()) {
				for (const auto scene = ProjectPlayerService::getInstance().getCurrentScene().value(); const auto &actor
				     : scene->getActors()) {
					if (actor->getProjectPed().has_value()) {
						if (ImGui::TreeNode(actor->getName().c_str())) {
							if (ImGui::Button(local.get("developer_dock.teleport_to_player").c_str())) {
								actor->getProjectPed().value()->Teleport(
									FindPlayerCoors(0) + CVector(1.f, 1.f, 0.f), false);
							}
							ImGui::SameLine();
							if (ImGui::Button(local.get("developer_dock.kill").c_str())) {
								Command<Commands::TASK_DEAD>(actor->getProjectPed().value());
							}

							ImGui::TreePop();
						}
					}
				}
			}
		}
		ImGui::End();
	}
}
