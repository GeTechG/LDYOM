#include "SetIncrementGlobalVariable.h"

#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utils.h"
#include "utilsRender.h"

SetIncrementGlobalVariable::SetIncrementGlobalVariable(void *_new): BaseObjective(_new) {
	const auto suffix =
		fmt::format(" : {}", Localization::getInstance().get("objective.set_increment_global_variable"));
	this->name += suffix;
}

ConditionActivator& SetIncrementGlobalVariable::getConditionActivator() { return conditionActivator; }

boost::uuids::uuid& SetIncrementGlobalVariable::getConditionActivatorUuid() { return conditionActivatorUuid; }

bool& SetIncrementGlobalVariable::isIncrement() { return increment; }
std::string& SetIncrementGlobalVariable::getVarUuid() { return varUuid; }

GlobalVariableView::Value& SetIncrementGlobalVariable::getVarValue() { return varValue; }

void SetIncrementGlobalVariable::draw(Localization &local, std::vector<std::string> &listOverlay) {
	utils::Combo(local.get("set_increment_global_variable.condition").c_str(),
	             reinterpret_cast<int*>(&this->conditionActivator),
	             local.getArray("set_increment_global_variable.conditions"));
	switch (this->conditionActivator) {
		case None:
			break;
		case DeadActor: {
			const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
			const int indexActor = utils::indexByUuid(actors, this->conditionActivatorUuid);

			IncorrectHighlight(indexActor == -1, [&] {
				utils::Combo(local.get("entities.actor").c_str(), &this->conditionActivatorUuid, indexActor,
				             actors.size(),
				             [&actors](const int i) {
					             return std::ref(actors.at(i)->getName());
				             }, [&actors](const int i) {
					             return actors.at(i)->getUuid();
				             });
			});

			if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
				if (indexActor == -1) {
					if (!actors.empty()) {
						this->conditionActivatorUuid = actors.back()->getUuid();
					}
				}
			}
			break;
		}
		case PlayerInCar: {
			const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
			const int indexVehicle = utils::indexByUuid(vehicles, this->conditionActivatorUuid);

			IncorrectHighlight(indexVehicle == -1, [&] {
				utils::Combo(local.get("entities.vehicle").c_str(), &this->conditionActivatorUuid, indexVehicle,
				             vehicles.size(),
				             [&vehicles](const int i) {
					             return std::ref(vehicles.at(i)->getName());
				             }, [&vehicles](const int i) {
					             return vehicles.at(i)->getUuid();
				             });
			});

			if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
				if (indexVehicle == -1) {
					if (!vehicles.empty()) {
						this->conditionActivatorUuid = vehicles.back()->getUuid();
					}
				}
			}
			break;
		}
	}

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

	if (!(this->varValue.getType() == GlobalVariableType::String ||
		this->varValue.getType() == GlobalVariableType::Bool)) {
		utils::ToggleButton(local.get("set_increment_global_variable.is_increment").c_str(), &this->increment);
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
}

ktwait SetIncrementGlobalVariable::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	if (this->varUuid.empty()) {
		setObjectiveError(result, *this, NotSelected, "The global variable for the objective is not selected.");
		co_return;
	}

	auto activateCallback = [&] {
		auto &globalVariablesManager = ProjectPlayerService::getInstance().getGlobalVariablesManager();
		const auto globalVariable = globalVariablesManager.getGlobalVariable(
			this->varUuid);
		if (globalVariable.getType() != this->getVarValue().getType()) {
			return;
		}
		switch (globalVariable.getType()) {
			case GlobalVariableType::Float: {
				if (this->increment) {
					globalVariablesManager.incrementGlobalVariable<float>(
						this->varUuid, *static_cast<float*>(this->getVarValue().value));
				} else {
					globalVariablesManager.setGlobalVariable<float>(
						this->varUuid, *static_cast<float*>(this->getVarValue().value));
				}
				break;
			}
			case GlobalVariableType::Int: {
				if (this->increment) {
					globalVariablesManager.incrementGlobalVariable<int>(
						this->varUuid, *static_cast<int*>(this->getVarValue().value));
				} else {
					globalVariablesManager.setGlobalVariable<int>(
						this->varUuid, *static_cast<int*>(this->getVarValue().value));
				}
				break;
			}
			case GlobalVariableType::String: {
				globalVariablesManager.setGlobalVariable<std::string>(
					this->varUuid, *static_cast<std::string*>(this->getVarValue().value));
				break;
			}
			case GlobalVariableType::Bool: {
				globalVariablesManager.setGlobalVariable<bool>(
					this->varUuid, *static_cast<bool*>(this->getVarValue().value));
				break;
			}
			default:
				break;
		}
	};

	switch (this->conditionActivator) {
		case None:
			activateCallback();
			break;
		case DeadActor: {
			const auto &actors = scene->getActors();
			const int indexActor = utils::indexByUuid(actors, this->conditionActivatorUuid);
			if (indexActor == -1) {
				setObjectiveError(result, *this, NotSelected, "The actor for the objective is not selected.");
				co_return;
			}
			const auto &actor = actors.at(indexActor);

			if (!actor->getProjectPed().has_value()) {
				setObjectiveError(result, *this, NotExists, "The entity of the actor does not exist.");
				co_return;
			}

			if (Command<Commands::IS_CHAR_DEAD>(actor->getProjectPed().value())) {
				activateCallback();
			}

			break;
		}
		case PlayerInCar: {
			const auto &vehicles = scene->getVehicles();
			const int indexVehicle = utils::indexByUuid(vehicles, this->conditionActivatorUuid);
			if (indexVehicle == -1) {
				setObjectiveError(result, *this, NotSelected, "The vehicle for the objective is not selected.");
				co_return;
			}
			const auto &vehicle = vehicles.at(indexVehicle);

			if (!vehicle->getProjectVehicle().has_value()) {
				setObjectiveError(result, *this, NotExists, "The entity of the vehicle does not exist.");
				co_return;
			}

			if (FindPlayerPed()->m_pVehicle != nullptr) {
				activateCallback();
			}

			break;
		}
	}
}
