#include "JumpToObjectiveObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

JumpToObjectiveObjective::JumpToObjectiveObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.jump_to_objective"));
	this->name += suffix;
}

boost::uuids::uuid& JumpToObjectiveObjective::getGoToObjectiveUuid() { return goToObjectiveUuid; }
boost::uuids::uuid& JumpToObjectiveObjective::getGoToObjectiveUuidElse() { return goToObjectiveUuidElse; }
bool& JumpToObjectiveObjective::isCondition() { return condition; }
int& JumpToObjectiveObjective::getConditionType() { return conditionType; }
std::string& JumpToObjectiveObjective::getVarUuid() { return varUuid; }
GlobalVariableView::Value& JumpToObjectiveObjective::getVarValue() { return varValue; }

void JumpToObjectiveObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &objectives = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives();
	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid);

	IncorrectHighlight(jumpObjectiveIdx == -1 && this->condition, [&] {
		utils::Combo(Localization::getInstance().get("objective.title").c_str(), &this->goToObjectiveUuid,
		             jumpObjectiveIdx, objectives.size(), [&objectives](const int i) {
			             return std::ref(objectives.at(i)->getName());
		             }, [&objectives](const int i) {
			             return objectives.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (jumpObjectiveIdx == -1) {
			if (!objectives.empty()) {
				this->goToObjectiveUuid = objectives.back()->getUuid();
			}
		}
	}

	utils::ToggleButton(local.get("general.condition").c_str(), &this->condition);

	ImGui::BeginDisabled(!this->condition);
	std::string previewVarName;
	if (!this->varUuid.empty()) {
		if (auto [result, var] = GlobalVariablesService::getInstance().get(this->varUuid); result == 0) {
			previewVarName = var.name;
		} else {
			this->varUuid.clear();
		}
	}

	IncorrectHighlight(this->varUuid.empty(), [&] {
		if (ImGui::BeginCombo(local.get("global_variables.global_variable").c_str(), previewVarName.c_str())) {
			auto &globalVariablesService = GlobalVariablesService::getInstance();
			for (int i = 0; i < globalVariablesService.getSize(); ++i) {
				auto [result, var] = globalVariablesService.get(i);
				const bool isSelected = var.uuid == this->varUuid;
				if (ImGui::Selectable(var.name.c_str(), isSelected)) {
					this->varUuid = var.uuid;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (this->varUuid.empty()) {
			if (auto [result, var] = GlobalVariablesService::getInstance().get(
				GlobalVariablesService::getInstance().getSize() - 1); result == 0) {
				this->varUuid = var.uuid;
			}
		}
	}


	if (this->varValue.getType() == GlobalVariableType::String ||
		this->varValue.getType() == GlobalVariableType::Bool) {
		ImGui::Dummy(ImVec2(
			ImGui::CalcItemWidth() / 2 - ImGui::CalcTextSize("==").x / 2 + ImGui::GetStyle().ItemSpacing.x / 2, 0));
		ImGui::SameLine();
		ImGui::Text("==");
	} else {
		utils::Combo(local.get("##condition").c_str(), &this->conditionType, local.getArray("general.compare_types"));
	}

	if (!this->varUuid.empty()) {
		if (auto [result, currentVar] = GlobalVariablesService::getInstance().get(this->varUuid); result == 0) {
			if (this->varValue.getType() != currentVar.value.getType()) {
				this->varValue.setType(currentVar.value.getType());
			}
			switch (this->varValue.getType()) {
				case GlobalVariableType::Float: {
					ImGui::InputFloat("##value", static_cast<float*>(this->varValue.value));
					break;
				}
				case GlobalVariableType::Int: {
					ImGui::InputInt("##value", static_cast<int*>(this->varValue.value), 0, 0);
					break;
				}
				case GlobalVariableType::String: {
					ImGui::InputText("##value", static_cast<std::string*>(this->varValue.value));
					break;
				}
				case GlobalVariableType::Bool: {
					const auto v = static_cast<bool*>(this->varValue.value);
					const auto booleanStrings = local.getArray("global_variables.bool_values");
					const std::string &label = *v ? booleanStrings[1] : booleanStrings[0];
					ImGui::Checkbox(label.c_str(), v);
					break;
				}
				default:
					break;
			}
		}
	}

	const int jumpObjectiveElseIdx = utils::indexByUuid(objectives, this->goToObjectiveUuidElse);
	IncorrectHighlight(jumpObjectiveElseIdx == -1 && this->condition, [&] {
		utils::Combo(Localization::getInstance().get("jump_to_objective.else_objective").c_str(),
		             &this->getGoToObjectiveUuidElse(),
		             jumpObjectiveElseIdx, objectives.size(), [&objectives](const int i) {
			             return std::ref(objectives.at(i)->getName());
		             }, [&objectives](const int i) {
			             return objectives.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (jumpObjectiveElseIdx == -1) {
			if (!objectives.empty()) {
				this->goToObjectiveUuidElse = objectives.back()->getUuid();
			}
		}
	}

	ImGui::EndDisabled();
}

ktwait JumpToObjectiveObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &objectives = scene->getObjectives();

	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid);
	const int jumpObjectiveElseIdx = utils::indexByUuid(objectives, this->goToObjectiveUuidElse);

	if (jumpObjectiveIdx == -1) {
		setObjectiveError(result, *this, NotSelected, "The objective is not selected.");
		co_return;
	}

	if (jumpObjectiveElseIdx == -1 && this->condition) {
		setObjectiveError(result, *this, NotSelected, "The else objective is not selected.");
		co_return;
	}

	if (this->condition) {
		if (this->varUuid.empty()) {
			setObjectiveError(result, *this, NotSelected, "The global variable is not selected.");
			co_return;
		}

		if (auto [r, var] = GlobalVariablesService::getInstance().get(this->varUuid); r == 0) {
			const auto variable = ProjectPlayerService::getInstance().getGlobalVariablesManager().
			                                                          getGlobalVariable(varUuid);
			switch (this->varValue.getType()) {
				case GlobalVariableType::Float: {
					if (utils::compare<float>(variable, this->varValue,
					                          static_cast<MathCondition>(this->conditionType))) {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
					} else {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveElseIdx);
					}
					break;
				}
				case GlobalVariableType::Int: {
					if (utils::compare<int>(variable, this->varValue,
					                        static_cast<MathCondition>(this->conditionType))) {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
					} else {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveElseIdx);
					}
					break;
				}
				case GlobalVariableType::String: {
					if (utils::compare<std::string>(variable, this->varValue, Equal)) {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
					} else {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveElseIdx);
					}
					break;
				}
				case GlobalVariableType::Bool: {
					if (utils::compare<bool>(variable, this->varValue, Equal)) {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
					} else {
						ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveElseIdx);
					}
					break;
				}
			}
		}
	} else {
		ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
	}

	co_return;
}
