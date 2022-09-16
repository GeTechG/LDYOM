#include "SetGlobalVariableObjective.h"

#include <sol.hpp>

#include "imgui.h"
#include "strUtils.h"
#include "../Windows/utilsRender.h"
#include "LuaEngine.h"
#include "NodeEditorUtils.h"
#include "utils.h"

SetGlobalVariableObjective::SetGlobalVariableObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.set_global_variable"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void SetGlobalVariableObjective::draw(Localization& local) {
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
		const auto type = static_cast<ValueType>(currentVariable.value()["type"].get<int>());
		if (type == ValueType::Number) {
			ImGui::InputFloat("##valueCompare", &this->valueNumber_);
		}
		else if (type == ValueType::Boolean) {
			ImGui::Checkbox("##valueCompare", &this->valueBoolean_);
		}
	}
}

ktwait SetGlobalVariableObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
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
		*varValue = this->valueNumber_;
	} else {
		const auto varValue = static_cast<bool*>(const_cast<void*>(LuaEngine::getInstance().getLuaState()["global_data"]["variablesR"][this->varId_].get<sol::object>().pointer()));
		*varValue = this->valueBoolean_;
	}

	co_return;
}
