#define IMGUI_DEFINE_MATH_OPERATORS
#include "scenes.h"
#include <fa_icons.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <logger.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>

void ScenesWindow::renderContent(ScenesWindow* window) {
	auto& scenesManager = ScenesManager::instance();
	auto& scenesInfo = scenesManager.getScenesInfo();

	if (scenesInfo.empty()) {
		ImGui::Text(_("scenes.no_scenes").c_str());
	} else {
		ImGui::BeginListBox("##ScenesList", ImVec2(0, 200) * SCL_PX);
		for (size_t i = 0; i < scenesInfo.size(); i++) {
			auto& sceneInfo = scenesInfo[i];
			ImGui::PushID(sceneInfo.id.c_str());
			auto isSelected = (window->m_selectedSceneIndex == static_cast<int>(i));
			auto isCurrentScene = (scenesManager.getCurrentScene().info.id == sceneInfo.id);
			if (ImGui::SelectableGTAColored((sceneInfo.name + (isCurrentScene ? _("scenes.current") : "")).c_str(),
			                                isSelected)) {
				window->m_selectedSceneIndex = static_cast<int>(i);
			}
			char renamePopupId[32];
			sprintf(renamePopupId, "rename_popup_%d", i);

			bool openRenamePopupIndex = false;
			static std::string renameBuffer = "";
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				renameBuffer = sceneInfo.name;
				openRenamePopupIndex = true;
			}

			if (openRenamePopupIndex) {
				ImGui::OpenPopup(renamePopupId);
			}

			if (ImGui::RenamePopup(renamePopupId, &renameBuffer)) {
				if (!renameBuffer.empty()) {
					if (isCurrentScene) {
						sceneInfo.name = renameBuffer;
						scenesManager.getUnsafeCurrentScene().info.name = renameBuffer;
					} else {
						sceneInfo.name = renameBuffer;
						scenesManager.rewriteSceneInfo(sceneInfo.id);
					}
				}
			}

			ImGui::PopID();
		}
		ImGui::EndListBox();
	}

	if (ImGui::Button(_("scenes.save").c_str())) {
		ScenesManager::instance().saveCurrentScene();
	}
	if (!scenesInfo.empty()) {
		ImGui::BeginDisabled(window->m_selectedSceneIndex < 0 ||
		                     window->m_selectedSceneIndex >= static_cast<int>(scenesInfo.size()));
		ImGui::SameLine();
		if (ImGui::Button(_("scenes.load").c_str())) {
			ScenesManager::instance().saveCurrentScene();
			ScenesManager::instance().loadScene(scenesInfo[window->m_selectedSceneIndex].id);
		}
		ImGui::SameLine();
		if (ImGui::Button(_("scenes.remove").c_str())) {
			window->m_indexToRemove = window->m_selectedSceneIndex;
		}
		ImGui::EndDisabled();
	}

	if (window->m_indexToRemove != -1) {
		auto state = ImGui::ConfirmDialog(_("scenes.remove_title").c_str(), _("scenes.remove_message").c_str());
		if (state == 1) {
			ScenesManager::instance().removeScene(scenesInfo[window->m_indexToRemove].id);
			window->m_selectedSceneIndex =
				std::min(window->m_selectedSceneIndex, static_cast<int>(scenesInfo.size() - 1));
			window->m_indexToRemove = -1;
		} else if (state == 0) {
			window->m_indexToRemove = -1;
		}
	}
}

ScenesWindow::ScenesWindow()
	: Window(_("scenes.title", ICON_FA_FILM)) {
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_AlwaysAutoResize);
	setRenderCallback<ScenesWindow>(ScenesWindow::renderContent);
}
