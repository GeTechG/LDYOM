#include "ScriptsWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <fmt/core.h>
#include "fa.h"
#include "imgui.h"
#include "LuaEngine.h"
#include "Settings.h"
#include "Localization/Localization.h"

void Windows::ScriptsWindow::Init() {
	if (auto loadedScripts = Settings::getInstance().get<std::list<std::string>>("data.loadedScripts");
		loadedScripts.has_value())
		this->loadedScripts.swap(loadedScripts.value());
	this->loadedScripts.remove_if([](const std::string &script) {
		return !LuaEngine::getInstance().getScriptsIds().contains(script);
	});
	Settings::getInstance().set("data.loadedScripts", this->loadedScripts);
	Settings::getInstance().Save();
	for (const auto &scriptsId : LuaEngine::getInstance().getScriptsIds()) {
		if (std::ranges::find(this->loadedScripts, scriptsId) ==
			this->loadedScripts.end()) {
			this->unloadedScripts.push_back(scriptsId);
		}
	}
}

void Windows::ScriptsWindow::draw() {
	static auto &local = Localization::getInstance();

	const auto &fontScale = ImGui::GetFontSize() / 16;
	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing,
	                        ImVec2(.5f, .5f));
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_SCROLL, local.get("scripts.title")).c_str(), nullptr,
	                 ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::BeginListBox("##unloadedScripts", ImVec2(200 * fontScale, 400 * fontScale))) {
			for (auto unloadedScriptIterate = unloadedScripts.begin();
			     unloadedScriptIterate != unloadedScripts.end();) {
				std::string name = LuaEngine::getInstance().getLuaState()["scripts"][*unloadedScriptIterate]["name"];
				if (ImGui::Selectable(name.c_str())) {
					this->loadedScripts.push_back(*unloadedScriptIterate);
					unloadedScriptIterate = this->unloadedScripts.erase(unloadedScriptIterate);
				} else {
					++unloadedScriptIterate;
				}
			}
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		if (ImGui::BeginListBox("##loadedScripts", ImVec2(200 * fontScale, 400 * fontScale))) {
			for (auto loadedScriptIterate = this->loadedScripts.begin();
			     loadedScriptIterate != this->loadedScripts.end();) {
				std::string name = LuaEngine::getInstance().getLuaState()["scripts"][*loadedScriptIterate]["name"];
				if (ImGui::Selectable(name.c_str())) {
					this->unloadedScripts.push_back(*loadedScriptIterate);
					loadedScriptIterate = this->loadedScripts.erase(loadedScriptIterate);
				} else {
					++loadedScriptIterate;
				}
			}
			ImGui::EndListBox();
		}
		{
			const auto calcTextSize = ImGui::CalcTextSize(local.get("general.save").c_str());
			const float spacing = ImGui::GetContentRegionAvail().x / 2 - (calcTextSize.x / 2 + ImGui::GetStyle().
				ItemSpacing.x);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spacing);
			if (ImGui::Button(local.get("general.save").c_str())) {
				Settings::getInstance().set("data.loadedScripts", this->loadedScripts);
				Settings::getInstance().Save();
				ImGui::OpenPopup("##infoSaveScripts");
			}
		}

		if (ImGui::BeginPopupModal("##infoSaveScripts", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(local.get("scripts.save_info").c_str());
			const auto calcTextSize = ImGui::CalcTextSize(local.get("general.ok").c_str());
			const float spacing = ImGui::GetContentRegionAvail().x / 2 - (calcTextSize.x / 2 + ImGui::GetStyle().
				ItemSpacing.x);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spacing);
			if (ImGui::Button(local.get("general.ok").c_str())) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
