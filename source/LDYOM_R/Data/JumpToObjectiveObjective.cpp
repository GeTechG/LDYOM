#include "JumpToObjectiveObjective.h"

#include "imgui.h"
#include "LuaEngine.h"
#include "NodeEditorUtils.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

JumpToObjectiveObjective::JumpToObjectiveObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.jump_to_objective"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void JumpToObjectiveObjective::draw(Localization& local) {
	const auto& objectives = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives();

	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid_);
	IncorrectHighlight(jumpObjectiveIdx == -1 && this->condition_, [&] {
		utils::Combo(Localization::getInstance().get("objective.title").c_str(), &this->goToObjectiveUuid_, jumpObjectiveIdx, static_cast<int>(objectives.size()), [&objectives](const int i) {
			return objectives.at(i)->getName();
			}, [&objectives](const int i) {
				return objectives.at(i)->getUuid();
			});
		});

	utils::ToggleButton(local.get("general.condition").c_str(), &this->condition_);

	ImGui::BeginDisabled(!this->condition_);

	std::optional<sol::table> currentVariable;
	for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["variables"].get_or_create<sol::table>()) {
		if (pair.first.as<int>() == this->varId_) {
			currentVariable = pair.second;
		}
	}

	const char* preview = "";
	if (currentVariable.has_value())
		preview = static_cast<const char*>(const_cast<void*>(currentVariable.value()["name"].get<sol::object>().pointer()));
	IncorrectHighlight(strlen(preview) == 0, [&] {
		if (ImGui::BeginCombo(local.get("general.global_variables").c_str(), preview)) {

			for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["variables"].get_or_create<sol::table>()) {
				auto variable = pair.second.as<sol::table>();
				auto type = static_cast<ValueType>(variable["type"].get<int>());
				if (type == ValueType::Number || type == ValueType::Boolean) {
					if (ImGui::Selectable(static_cast<char*>(const_cast<void*>(variable["name"].get<sol::object>().pointer())), this->varId_ == pair.first.as<int>())) {
						this->varId_ = pair.first.as<int>();
					}
				}
			}

			ImGui::EndCombo();
		}
	});
	
	if (currentVariable.has_value()) {
		if (utils::Combo(local.get("general.condition").c_str(), &this->conditionType_, local.getArray("general.comparings"))) {
			this->conditionValueNumber_ = 0;
		}
		const auto type = static_cast<ValueType>(currentVariable.value()["type"].get<int>());
		if (type == ValueType::Number) {
			ImGui::InputFloat("##valueCompare", &this->conditionValueNumber_);
		}
		else if (type == ValueType::Boolean) {
			ImGui::Checkbox("##valueCompare", &this->conditionValueBoolean_);
		}
	}

	ImGui::EndDisabled();
}

ktwait JumpToObjectiveObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	const auto& objectives = scene->getObjectives();

	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid_);

	if (jumpObjectiveIdx == -1) {
		setObjectiveError(result, *this, NotSelected, "The objective is not selected.");
		co_return;
	}

	if (this->condition_) {
		std::optional<sol::table> currentVariable;
		for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["variables"].get_or_create<sol::table>()) {
			if (pair.first.as<int>() == this->varId_) {
				currentVariable = pair.second;
			}
		}

		if (!currentVariable.value()) {
			setObjectiveError(result, *this, ErrorCode::NotSelected, "The global variable is not selected.");
			co_return;
		}
		
		const auto type = static_cast<ValueType>(currentVariable.value()["type"].get<int>());
		if (type == ValueType::Number) {
			const auto varValue = static_cast<float*>(const_cast<void*>(LuaEngine::getInstance().getLuaState()["global_data"]["variablesR"][this->varId_].get<sol::object>().pointer()));
			switch (this->conditionType_) {
			case 0:
				if (abs(*varValue - this->conditionValueNumber_) < FLT_EPSILON) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 1:
				if (abs(*varValue - this->conditionValueNumber_) > FLT_EPSILON) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 2:
				if (*varValue > this->conditionValueNumber_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 3:
				if (*varValue >= this->conditionValueNumber_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 4:
				if (*varValue < this->conditionValueNumber_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 5:
				if (*varValue <= this->conditionValueNumber_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			default:
				break;
			}
		}
		else if (type == ValueType::Boolean) {
			const auto varValue = static_cast<bool*>(const_cast<void*>(LuaEngine::getInstance().getLuaState()["global_data"]["variablesR"][this->varId_].get<sol::object>().pointer()));
			switch (this->conditionType_) {
			case 0:
				if (*varValue == this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 1:
				if (*varValue != this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 2:
				if (*varValue > this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 3:
				if (*varValue >= this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 4:
				if (*varValue < this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			case 5:
				if (*varValue <= this->conditionValueBoolean_) {
					ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
				}
				break;
			default:
				break;
			}
		}
	} else {
		ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
	}

	co_return;
}
