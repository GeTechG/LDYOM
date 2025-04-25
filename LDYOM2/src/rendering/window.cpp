#define IMGUI_DEFINE_MATH_OPERATORS
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
	ImGui::SetNextWindowPos(this->m_position * screenScale, ImGuiCond_FirstUseEver, this->m_pivot);
	ImGui::SetNextWindowSize(this->m_size * screenScale, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(this->m_sizeMin * screenScale, ImVec2(FLT_MAX, FLT_MAX));
}

void Window::render() {
	if (m_open) {
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
