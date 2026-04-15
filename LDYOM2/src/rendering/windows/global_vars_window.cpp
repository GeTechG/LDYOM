#define IMGUI_DEFINE_MATH_OPERATORS
#include "global_vars_window.h"
#include "data/global_variable.h"
#include <global_vars_service.h>
#include <fa_icons.h>
#include <imgui_stdlib.h>
#include <localization.h>
#include <logger.h>
#include <projects_manager.h>
#include <utils/imgui_configurate.h>
#include <utils/ui_scale.h>

static const char* typeLabels[] = {"Number", "String", "Boolean"};

void GlobalVarsWindow::renderContent(GlobalVarsWindow* window) {
	auto projOpt = ProjectsManager::instance().getCurrentProject();
	if (!projOpt.has_value()) {
		ImGui::TextDisabled("No project loaded.");
		return;
	}
	auto* proj = projOpt.value();
	auto& vars = proj->globalVars;

	bool dirty = false;

	// ── Add Variable ────────────────────────────────────────────────────
	ImGui::SetNextItemWidth(ui::em(12.5f));
	ImGui::InputTextWithHint("##new_var_name", _("global_vars.name").c_str(), &window->m_newVarName);
	ImGui::SameLine();
	ImGui::BeginDisabled(window->m_newVarName.empty());
	if (ImGui::Button(_("global_vars.add_variable", ICON_FA_PLUS).c_str())) {
		GlobalVariable gv;
		gv.name = window->m_newVarName;
		vars.push_back(gv);
		window->m_newVarName.clear();
		dirty = true;
	}
	ImGui::EndDisabled();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// ── Table ────────────────────────────────────────────────────────────
	if (vars.empty()) {
		float w = ImGui::GetContentRegionAvail().x;
		auto txt = _("global_vars.empty");
		ImGui::SetCursorPosX((w - ImGui::CalcTextSize(txt.c_str()).x) * 0.5f);
		ImGui::TextDisabled("%s", txt.c_str());
	} else {
		const ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
		                                   ImGuiTableFlags_SizingStretchSame;
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(6, 4));
		if (ImGui::BeginTable("GlobalVarsTable", 4, tableFlags, ImVec2(0, ui::em(17.5f)))) {
			ImGui::TableSetupColumn(_("global_vars.name").c_str(), ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(_("global_vars.type").c_str(), ImGuiTableColumnFlags_WidthFixed, ui::em(6.0f));
			ImGui::TableSetupColumn(_("global_vars.value").c_str(), ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(_("global_vars.delete").c_str(), ImGuiTableColumnFlags_WidthFixed, ui::em(3.0f));
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			for (int i = 0; i < static_cast<int>(vars.size()); ++i) {
				auto& var = vars[i];
				ImGui::PushID(i);
				ImGui::TableNextRow();

				// Name
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1.0f);
				if (ImGui::InputText("##name", &var.name))
					dirty = true;

				// Type
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1.0f);
				int typeIdx = static_cast<int>(var.type());
				if (ImGui::Combo("##type", &typeIdx, typeLabels, 3)) {
					GlobalVarType newType = static_cast<GlobalVarType>(typeIdx);
					if (newType != var.type()) {
						switch (newType) {
							case GlobalVarType::Number:  var.defaultValue = 0.0;             break;
							case GlobalVarType::String:  var.defaultValue = std::string{};   break;
							case GlobalVarType::Boolean: var.defaultValue = false;            break;
						}
						dirty = true;
					}
				}

				// Default Value
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1.0f);
				switch (var.type()) {
					case GlobalVarType::Number:
						if (ImGui::InputDouble("##val", &std::get<double>(var.defaultValue)))
							dirty = true;
						break;
					case GlobalVarType::String:
						if (ImGui::InputText("##val", &std::get<std::string>(var.defaultValue)))
							dirty = true;
						break;
					case GlobalVarType::Boolean:
						if (ImGui::Checkbox("##val", &std::get<bool>(var.defaultValue)))
							dirty = true;
						break;
				}

				// Delete
				ImGui::TableNextColumn();
				if (ImGui::SmallButton(fmt::format("{}##del", ICON_FA_TRASH).c_str()))
					window->m_pendingDeleteIndex = i;

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
	}

	// Deferred delete
	if (window->m_pendingDeleteIndex >= 0 && window->m_pendingDeleteIndex < static_cast<int>(vars.size())) {
		vars.erase(vars.begin() + window->m_pendingDeleteIndex);
		window->m_pendingDeleteIndex = -1;
		dirty = true;
	}

	if (dirty) {
		ProjectsManager::instance().saveCurrentProject();
		GlobalVarsService::instance().reset();
	}

	// ── Close ─────────────────────────────────────────────────────────────
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	float bw = ui::em(7.5f);
	ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - bw) * 0.5f);
	if (ImGui::Button(_("tools.close").c_str(), ImVec2(bw, 0)))
		window->close();
}

GlobalVarsWindow::GlobalVarsWindow()
	: ModalPopupWindow(_("global_vars.title", ICON_FA_DATABASE)) {
	setSize(600, 420);
	setSizeMin(450, 300);
	setPosition(640, 360);
	setPivot(0.5f, 0.5f);
	setRenderCallback<GlobalVarsWindow>(GlobalVarsWindow::renderContent);
}
