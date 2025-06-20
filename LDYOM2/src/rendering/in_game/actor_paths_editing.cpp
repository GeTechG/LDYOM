#define IMGUI_DEFINE_MATH_OPERATORS
#include "actor_paths_editing.h"
#include <CPools.h>
#include <CSprite.h>
#include <CStreaming.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui_hook/utils/imgui_configurate.h>
#include <localization.h>
#include <models_manager.h>
#include <utils/task_manager.h>
#include <window_manager.h>

PointsArray ActorPathsEditing::m_points;
std::function<void(bool, const PointsArray&)> ActorPathsEditing::m_onCloseCallback;
size_t ActorPathsEditing::m_currentPointIndex = 0;
bool ActorPathsEditing::giveJetpack = false;

void ActorPathsEditing::render() noexcept {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	if (ImGui::Begin("##ActorPathsEditor", nullptr, windowFlags)) {
		const auto textWidth = (SCL_PX).x * 300.f;
		ImGui::PushTextWrapPos(textWidth);
		ImGui::TextWrapped(_("actor_paths_editing.info_add_point", "N").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_remove_point", "M").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_clear_all_points", "L").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_move_point_next", "]").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_move_point_prev", "[").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_update_point", "U").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_cancel_editing", "F").c_str());
		ImGui::TextWrapped(_("actor_paths_editing.info_save", "Y").c_str());
		ImGui::PopTextWrapPos();
	}

	auto drawlist = ImGui::GetBackgroundDrawList();
	if (!m_points.empty()) {
		RwV3d lastPoint;
		float w, h;
		auto lastVisible = CSprite::CalcScreenCoors(RwV3d{m_points.at(0)[0], m_points.at(0)[1], m_points.at(0)[2]},
		                                            &lastPoint, &w, &h, true, true);
		if (lastVisible)
			drawlist->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
			                          m_currentPointIndex == 0 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
		for (size_t i = 1; i < m_points.size(); ++i) {
			RwV3d currentPoint;
			const auto currentVisible = CSprite::CalcScreenCoors(
				RwV3d{m_points.at(i)[0], m_points.at(i)[1], m_points.at(i)[2]}, &currentPoint, &w, &h, true, true);
			if (lastVisible && currentVisible)
				drawlist->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
				                  IM_COL32_WHITE);
			if (currentVisible)
				drawlist->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
				                          m_currentPointIndex == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
			lastPoint = currentPoint;
			lastVisible = currentVisible;
		}
	}

	if (ImGui::IsKeyReleased(ImGuiKey_N)) {
		const auto p = FindPlayerPed()->GetPosition();
		m_points.push_back({p.x, p.y, p.z});
	}
	if (ImGui::IsKeyReleased(ImGuiKey_M)) {
		if (!m_points.empty() && m_currentPointIndex < m_points.size()) {
			m_points.erase(m_points.begin() + m_currentPointIndex);
			if (m_currentPointIndex >= m_points.size()) {
				m_currentPointIndex = m_points.size() - 1;
			}
		}
	}
	if (ImGui::IsKeyReleased(ImGuiKey_L)) {
		m_points.clear();
		m_currentPointIndex = 0;
	}
	if (ImGui::IsKeyReleased(ImGuiKey_RightBracket)) {
		if (m_currentPointIndex < m_points.size() - 1) {
			m_currentPointIndex++;
		}
	}
	if (ImGui::IsKeyReleased(ImGuiKey_LeftBracket)) {
		if (m_currentPointIndex > 0) {
			m_currentPointIndex--;
		}
	}
	if (ImGui::IsKeyReleased(ImGuiKey_U)) {
		if (m_currentPointIndex < m_points.size()) {
			const auto p = FindPlayerPed()->GetPosition();
			m_points[m_currentPointIndex] = {p.x, p.y, p.z};
		}
	}
	if (ImGui::IsKeyReleased(ImGuiKey_F)) {
		closePathEditor(false);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_Y)) {
		closePathEditor(true);
	}

	ImGui::End();
}

void ActorPathsEditing::openPathEditor(const PointsArray& points,
                                       std::function<void(bool, const PointsArray&)> onClose) noexcept {
	m_points = points;
	m_onCloseCallback = std::move(onClose);
	if (giveJetpack) {
		plugin::Command<plugin::Commands::TASK_JETPACK>(CPools::GetPedRef(FindPlayerPed()));
	}
	WindowManager::instance().addBackgroundRenderCallback("ActorPathsEditor", render);
}

void ActorPathsEditing::closePathEditor(bool saveChanges) noexcept {
	if (m_onCloseCallback) {
		m_onCloseCallback(saveChanges, m_points);
		m_onCloseCallback = nullptr;
	}
	if (giveJetpack) {
		plugin::Command<plugin::Commands::REMOVE_ALL_CHAR_WEAPONS>(CPools::GetPedRef(FindPlayerPed()));
		giveJetpack = false;
	}
	WindowManager::instance().removeBackgroundRenderCallback("ActorPathsEditor");
}