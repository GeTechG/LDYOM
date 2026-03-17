#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "data/global_variable.h"
#include "objective.h"
#include <global_vars_service.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <projects_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::set_global_var {
constexpr const char* TYPE = "core.set_global_var";

enum class Operation { Assign = 0, Add = 1, Subtract = 2 };

NLOHMANN_JSON_SERIALIZE_ENUM(Operation, {
											{Operation::Assign, "assign"},
											{Operation::Add, "add"},
											{Operation::Subtract, "subtract"},
										})

struct Data {
	std::string varName;
	Operation operation = Operation::Assign;
	double numberValue = 0.0;
	std::string stringValue;
	bool boolValue = false;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveSetGlobalVarData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, varName, numberValue, stringValue, boolValue);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, varName, operation, numberValue, stringValue, boolValue)
};

// Returns GlobalVarType for varName, or Number if not found
static GlobalVarType findVarType(const std::string& varName) {
	auto projOpt = ProjectsManager::instance().getCurrentProject();
	if (projOpt.has_value()) {
		for (const auto& gv : projOpt.value()->globalVars) {
			if (gv.name == varName)
				return gv.type();
		}
	}
	return GlobalVarType::Number;
}

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));

	auto projOpt = ProjectsManager::instance().getCurrentProject();

	// ── Variable ─────────────────────────────────────────────────────────
	ImGui::Text(_("objectives.core.set_global_var.variable").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const char* preview = data.varName.empty() ? "..." : data.varName.c_str();
	if (ImGui::BeginCombo("##varname", preview)) {
		if (projOpt.has_value()) {
			for (const auto& gv : projOpt.value()->globalVars) {
				if (ImGui::Selectable(gv.name.c_str(), data.varName == gv.name)) {
					if (data.varName != gv.name) {
						data.varName = gv.name;
						if (gv.type() != GlobalVarType::Number)
							data.operation = Operation::Assign;
					}
				}
			}
		}
		ImGui::EndCombo();
	}

	GlobalVarType curType = findVarType(data.varName);

	// ── Operation (numbers only) ──────────────────────────────────────────
	if (curType == GlobalVarType::Number) {
		ImGui::Text(_("objectives.core.set_global_var.operation").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		int opIdx = static_cast<int>(data.operation);
		if (ImGui::BeginCombo("##operation",
		                      _("objectives.core.set_global_var.operations." + std::to_string(opIdx)).c_str())) {
			for (int i = 0; i < 3; ++i) {
				if (ImGui::Selectable(_("objectives.core.set_global_var.operations." + std::to_string(i)).c_str(),
				                      opIdx == i)) {
					data.operation = static_cast<Operation>(i);
				}
			}
			ImGui::EndCombo();
		}
	}

	// ── Value ─────────────────────────────────────────────────────────────
	ImGui::Text(_("objectives.core.set_global_var.value").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	switch (curType) {
		case GlobalVarType::Number: ImGui::InputDouble("##val", &data.numberValue); break;
		case GlobalVarType::String: ImGui::InputText("##val", &data.stringValue); break;
		case GlobalVarType::Boolean: ImGui::Checkbox("##val", &data.boolValue); break;
	}
}

ktwait execute(Data& data) {
	switch (findVarType(data.varName)) {
		case GlobalVarType::Number:
			switch (data.operation) {
				case Operation::Assign: GlobalVarsService::instance().setNumber(data.varName, data.numberValue); break;
				case Operation::Add: GlobalVarsService::instance().addNumber(data.varName, data.numberValue); break;
				case Operation::Subtract:
					GlobalVarsService::instance().addNumber(data.varName, -data.numberValue);
					break;
			}
			break;
		case GlobalVarType::String: GlobalVarsService::instance().setString(data.varName, data.stringValue); break;
		case GlobalVarType::Boolean: GlobalVarsService::instance().setBool(data.varName, data.boolValue); break;
	}
	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::set_global_var
