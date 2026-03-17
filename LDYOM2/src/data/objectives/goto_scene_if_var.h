#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "data/global_variable.h"
#include "objective.h"
#include <global_vars_service.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <project_player.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::goto_scene_if_var {
constexpr const char* TYPE = "core.goto_scene_if_var";

enum class Condition { Equal = 0, NotEqual = 1, Greater = 2, GreaterEqual = 3, Less = 4, LessEqual = 5 };

NLOHMANN_JSON_SERIALIZE_ENUM(Condition, {
											{Condition::Equal, "eq"},
											{Condition::NotEqual, "neq"},
											{Condition::Greater, "gt"},
											{Condition::GreaterEqual, "gte"},
											{Condition::Less, "lt"},
											{Condition::LessEqual, "lte"},
										})

struct Data {
	std::string sceneId;
	std::string varName;
	Condition condition = Condition::Equal;
	double numberValue = 0.0;
	std::string stringValue;
	bool boolValue = false;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveGotoSceneIfVarData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, sceneId, varName, numberValue, stringValue, boolValue);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, sceneId, varName, condition, numberValue, stringValue, boolValue)
};

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

	// ── Scene ─────────────────────────────────────────────────────────────
	const auto& scenes = ScenesManager::instance().getScenesInfo();
	std::string scenePreview;
	for (const auto& scene : scenes) {
		if (scene.id == data.sceneId) {
			scenePreview = scene.name;
			break;
		}
	}
	ImGui::Text(_("objectives.core.goto_scene_if_var.scene").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##scene", scenePreview.empty() ? "..." : scenePreview.c_str())) {
		for (const auto& scene : scenes) {
			bool selected = scene.id == data.sceneId;
			if (ImGui::Selectable(scene.name.c_str(), selected))
				data.sceneId = scene.id;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// ── Variable ─────────────────────────────────────────────────────────
	ImGui::Text(_("objectives.core.goto_scene_if_var.variable").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const char* varPreview = data.varName.empty() ? "..." : data.varName.c_str();
	auto projOpt = ProjectsManager::instance().getCurrentProject();
	if (ImGui::BeginCombo("##varname", varPreview)) {
		if (projOpt.has_value()) {
			for (const auto& gv : projOpt.value()->globalVars) {
				if (ImGui::Selectable(gv.name.c_str(), data.varName == gv.name)) {
					if (data.varName != gv.name) {
						data.varName = gv.name;
						data.condition = Condition::Equal;
					}
				}
			}
		}
		ImGui::EndCombo();
	}

	GlobalVarType curType = findVarType(data.varName);

	// ── Condition ─────────────────────────────────────────────────────────
	ImGui::Text(_("objectives.core.goto_scene_if_var.condition").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	int condIdx = static_cast<int>(data.condition);

	if (curType == GlobalVarType::Boolean) {
		// For booleans only Equal/NotEqual make sense
		if (condIdx > 1)
			condIdx = 0;
		if (ImGui::BeginCombo("##condition",
		                      _("objectives.core.goto_scene_if_var.conditions." + std::to_string(condIdx)).c_str())) {
			for (int i = 0; i < 2; ++i) {
				if (ImGui::Selectable(
				        _("objectives.core.goto_scene_if_var.conditions." + std::to_string(i)).c_str(), condIdx == i))
					data.condition = static_cast<Condition>(i);
			}
			ImGui::EndCombo();
		}
	} else if (curType == GlobalVarType::String) {
		if (condIdx > 1)
			condIdx = 0;
		if (ImGui::BeginCombo("##condition",
		                      _("objectives.core.goto_scene_if_var.conditions." + std::to_string(condIdx)).c_str())) {
			for (int i = 0; i < 2; ++i) {
				if (ImGui::Selectable(
				        _("objectives.core.goto_scene_if_var.conditions." + std::to_string(i)).c_str(), condIdx == i))
					data.condition = static_cast<Condition>(i);
			}
			ImGui::EndCombo();
		}
	} else {
		if (ImGui::BeginCombo("##condition",
		                      _("objectives.core.goto_scene_if_var.conditions." + std::to_string(condIdx)).c_str())) {
			for (int i = 0; i < 6; ++i) {
				if (ImGui::Selectable(
				        _("objectives.core.goto_scene_if_var.conditions." + std::to_string(i)).c_str(), condIdx == i))
					data.condition = static_cast<Condition>(i);
			}
			ImGui::EndCombo();
		}
	}

	// ── Value ─────────────────────────────────────────────────────────────
	ImGui::Text(_("objectives.core.goto_scene_if_var.value").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	switch (curType) {
		case GlobalVarType::Number: ImGui::InputDouble("##val", &data.numberValue); break;
		case GlobalVarType::String: ImGui::InputText("##val", &data.stringValue); break;
		case GlobalVarType::Boolean: ImGui::Checkbox("##val", &data.boolValue); break;
	}
}

static bool evaluateCondition(const Data& data) {
	GlobalVarType curType = findVarType(data.varName);
	switch (curType) {
		case GlobalVarType::Number: {
			double current = GlobalVarsService::instance().getNumber(data.varName);
			switch (data.condition) {
				case Condition::Equal: return current == data.numberValue;
				case Condition::NotEqual: return current != data.numberValue;
				case Condition::Greater: return current > data.numberValue;
				case Condition::GreaterEqual: return current >= data.numberValue;
				case Condition::Less: return current < data.numberValue;
				case Condition::LessEqual: return current <= data.numberValue;
			}
			break;
		}
		case GlobalVarType::String: {
			std::string current = GlobalVarsService::instance().getString(data.varName);
			switch (data.condition) {
				case Condition::Equal: return current == data.stringValue;
				case Condition::NotEqual: return current != data.stringValue;
				default: return current == data.stringValue;
			}
			break;
		}
		case GlobalVarType::Boolean: {
			bool current = GlobalVarsService::instance().getBool(data.varName);
			switch (data.condition) {
				case Condition::Equal: return current == data.boolValue;
				case Condition::NotEqual: return current != data.boolValue;
				default: return current == data.boolValue;
			}
			break;
		}
	}
	return false;
}

ktwait execute(Data& data) {
	if (evaluateCondition(data))
		ProjectPlayer::instance().requestSceneTransition(data.sceneId);
	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::goto_scene_if_var
