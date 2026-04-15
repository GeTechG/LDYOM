#define IMGUI_DEFINE_MATH_OPERATORS
#include <tracy/Tracy.hpp>
#include "window.h"
#include "utils/imgui_configurate.h"
#include <imgui.h>
#include <localization.h>

void Window::open() {
	m_open = true;
	m_isAppering = true;
}

void Window::close() { m_open = false; }

void Window::configurateWindow() {
	const float uiScale = ImGuiConfigurate::getUiScale();

	ImGuiCond sizeCond = ImGuiCond_FirstUseEver;
	ImVec2 nextSize = this->m_size * uiScale;
	if (m_lastUiScale > 0.0f && m_lastUiScale != uiScale && (m_lastPixelSize.x > 0.0f || m_lastPixelSize.y > 0.0f)) {
		sizeCond = ImGuiCond_Always;
		const float ratio = uiScale / m_lastUiScale;
		nextSize = ImVec2(m_lastPixelSize.x * ratio, m_lastPixelSize.y * ratio);
	}
	m_lastUiScale = uiScale;

	const ImVec2 screenScale = ImGuiConfigurate::getScreenScale();
	const ImVec2 scaledPos = ImVec2(this->m_position.x * screenScale.x, this->m_position.y * screenScale.y);
	ImGui::SetNextWindowPos(scaledPos, this->m_positionCondition, this->m_pivot);
	ImGui::SetNextWindowSize(nextSize, sizeCond);
	ImGui::SetNextWindowSizeConstraints(this->m_sizeMin * uiScale, ImVec2(FLT_MAX, FLT_MAX));
}

void Window::render() {
	if (m_open) {
		ZoneScopedN("Window::render");
		ZoneText(m_title.c_str(), m_title.size());
		this->configurateWindow();
		if (ImGui::Begin(_(this->m_title).c_str(), &this->m_open, this->m_flags)) {
			this->m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
			m_lastPixelSize = ImGui::GetWindowSize();

			if (m_renderCallback) {
				m_renderCallback(this);
			}
		}
		ImGui::End();
		this->m_isAppering = false;
	}
}
