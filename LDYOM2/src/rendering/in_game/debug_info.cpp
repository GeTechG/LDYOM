#define IMGUI_DEFINE_MATH_OPERATORS
#include "debug_info.h"
#include <hotkeys.h>
#include <imHotKey.h>
#include <imgui_hook/utils/imgui_configurate.h>
#include <localization.h>
#include <project_player.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <window_manager.h>

bool DebugInfo::m_isVisible = false;

void DebugInfo::render() noexcept {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

	const auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always, ImVec2(0.0f, 0.0f)); // Top-left corner

	if (ImGui::Begin("##DebugInfo", nullptr, windowFlags)) {
		const auto& player = ProjectPlayer::instance();
		auto& projectsManager = ProjectsManager::instance();
		const auto& scenesManager = ScenesManager::instance();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), _("debug_info.header").c_str());

		if (player.isPlaying()) {
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), _("debug_info.playing_yes").c_str());

			// Current objective info
			const auto currentObjIndex = player.getCurrentObjectiveIndex();
			const auto& currentScene = scenesManager.getCurrentScene();
			const auto totalObjectives = currentScene.objectives.data.size();
			ImGui::Text(_("debug_info.objective", currentObjIndex + 1, totalObjectives).c_str());

			// Scene info
			const auto& sceneId = player.getCurrentSceneId();
			ImGui::Text(_("debug_info.scene", sceneId).c_str());

			// Fade state
			const auto isFaded = player.isFaded();
			ImGui::TextColored(isFaded ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
			                   isFaded ? _("debug_info.faded_yes").c_str() : _("debug_info.faded_no").c_str());
		} else {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), _("debug_info.playing_no").c_str());
		}

		// Project info
		const auto projectInfo = projectsManager.getCurrentProject();
		if (projectInfo.has_value()) {
			ImGui::Separator();
			ImGui::Text(_("debug_info.project", projectInfo.value()->name).c_str());
		}

		// Get hotkey name dynamically
		auto hotkeyOpt = Hotkeys::instance().getHotKeyByName("toggleDebugInfo");
		char hotkeyLib[128] = "F3";
		if (hotkeyOpt.has_value()) {
			ImHotKey::GetHotKeyLib(hotkeyOpt.value().get().functionKeys, hotkeyLib, sizeof(hotkeyLib));
		}

		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), _("debug_info.press_to_hide", hotkeyLib).c_str());
	}
	ImGui::End();
}

void DebugInfo::toggle() noexcept {
	m_isVisible = !m_isVisible;

	if (m_isVisible) {
		WindowManager::instance().addBackgroundRenderCallback("DebugInfo", render);
	} else {
		WindowManager::instance().removeBackgroundRenderCallback("DebugInfo");
	}
}
