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
	const auto screenScale = ImGuiConfigurate::getScreenScale();
	const float globalScale = ImGuiConfigurate::getGlobalScale();
	const ImVec2 totalScale = screenScale * globalScale;

	ImGuiCond sizeCond = ImGuiCond_FirstUseEver;
	ImVec2 nextSize = this->m_size * screenScale;
	if (m_lastGlobalScale > 0.0f && m_lastGlobalScale != globalScale) {
		sizeCond = ImGuiCond_Always;
		const float ratio = globalScale / m_lastGlobalScale;
		nextSize = ImVec2(this->m_size.x * ratio, this->m_size.y * ratio);
	}
	m_lastGlobalScale = globalScale;

	ImGui::SetNextWindowPos(this->m_position * totalScale, this->m_positionCondition, this->m_pivot);
	ImGui::SetNextWindowSize(nextSize, sizeCond);
	ImGui::SetNextWindowSizeConstraints(this->m_sizeMin * totalScale, ImVec2(FLT_MAX, FLT_MAX));
}

void Window::render() {
	if (m_open) {
		ZoneScopedN("Window::render");
		ZoneText(m_title.c_str(), m_title.size());
		this->configurateWindow();
		if (ImGui::Begin(_(this->m_title).c_str(), &this->m_open, this->m_flags)) {
			this->m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
			ImVec2 currentSize = ImGui::GetWindowSize();
			if (currentSize.x != this->m_size.x || currentSize.y != this->m_size.y) {
				this->m_size = currentSize;
			}

			if (m_renderCallback) {
				m_renderCallback(this);
			}
		}
		ImGui::End();
		this->m_isAppering = false;
	}
}
