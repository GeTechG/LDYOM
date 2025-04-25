#include "modal_popup_window.h"

ModalPopupWindow::ModalPopupWindow(const std::string& title)
	: Window(title) {}

void ModalPopupWindow::render() {
	if (m_needOpen) {
		this->m_open = true;
		ImGui::OpenPopup(this->m_title.c_str());
		this->m_needOpen = false;
	}
	if (m_open) {
		this->configurateWindow();
		if (ImGui::BeginPopupModal(this->m_title.c_str(), &this->m_open, this->m_flags)) {
			ImVec2 currentSize = ImGui::GetWindowSize();
			if (currentSize.x != this->m_size.x || currentSize.y != this->m_size.y) {
				// this->onResize(currentSize);
				this->m_size = currentSize;
			}

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
	this->m_isAppering = true;
}

void ModalPopupWindow::close() { this->m_needClose = true; }
