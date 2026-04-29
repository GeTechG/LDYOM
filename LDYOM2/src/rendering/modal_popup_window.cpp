#include <tracy/Tracy.hpp>
#include "modal_popup_window.h"

ModalPopupWindow::ModalPopupWindow(const std::string& title)
	: Window(title) {}

void ModalPopupWindow::render() {
	ZoneScopedN("ModalPopupWindow::render");
	ZoneText(m_title.c_str(), m_title.size());
	if (m_needOpen) {
		this->m_open = true;
		ImGui::OpenPopup(this->m_title.c_str());
		this->m_needOpen = false;
	}
	if (m_open) {
		this->configurateWindow();
		if (ImGui::BeginPopupModal(this->m_title.c_str(), &this->m_open, this->m_flags)) {
			m_lastPixelSize = ImGui::GetWindowSize();

			if (m_renderCallback) {
				m_renderCallback(this);
			}
			if (m_needClose) {
				this->m_open = false;
				ImGui::CloseCurrentPopup();
				this->m_needClose = false;
			}
			ImGui::EndPopup();
		}
		this->m_isAppering = false;
	}
}

void ModalPopupWindow::open() {
	this->m_needOpen = true;
	// Also flip m_open synchronously so isAnyWindowOpen()/the render-loop gate count this
	// popup as open before its render() runs and consumes m_needOpen. Otherwise opening a
	// modal popup while no other window is m_open=true (e.g. Welcome → QuickSettings handoff)
	// leaves the loop with nothing to render and the popup never mounts.
	this->m_open = true;
	this->m_isAppering = true;
}

void ModalPopupWindow::close() { this->m_needClose = true; }
