#include "ListWindow.h"
#include "constants.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Localization/Localization.h"

void renamePopup(std::string &name) {
	if (ImGui::BeginPopup("##renameElementPopup")) {
		ImGui::InputText("##inputNameRename", &name, ImGuiInputTextFlags_EnterReturnsTrue);
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

ImVec2 Windows::ListWindow::drawList() {
	static auto &local = Localization::getInstance();

	const auto windowSize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos(ImVec2(windowSize.x, 0.0f), ImGuiCond_Appearing, ImVec2(1.0f, 0.0f));
	if (ImGui::Begin(this->getNameList().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		const float listWidth = windowSize.x / 3.8f;
		const float listHeight = windowSize.y / 2.0f;
		const auto listSize = ImVec2(listWidth, listHeight);

		if (ImGui::BeginChild("##ListElements", listSize, true)) {
			for (int i = 0; i < this->getListSize(); ++i) {
				ImGui::PushID(i);
				if (ImGui::Selectable(this->getElementName(i).c_str(), this->currentElement == i)) {
					selectElement(i);
				}

				if (this->dragsItems) {
					if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
						const int nextItem = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
						if (nextItem >= 0 && nextItem < this->getListSize()) {
							this->swapItems(i, nextItem);
							ImGui::ResetMouseDragDelta();
						}
					}
				}

				if (ImGui::BeginPopupContextItem("##editElementName")) {
					ImGui::InputText("##inputNameRename", &this->getElementName(i),
					                 ImGuiInputTextFlags_EnterReturnsTrue);
					if (ImGui::IsItemDeactivatedAfterEdit()) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
			ImGui::EndChild();
		}

		const float buttonWidth = listSize.x / 3.0f - 3.0f;

		if (ImGui::Button(local.get("list_window.create_new").c_str(), ImVec2(buttonWidth, 0))) {
			onButtonCreateNewElement();
		}
		ImGui::SameLine();
		ImGui::BeginDisabled(this->currentElement == -1);
		if (ImGui::Button(local.get("list_window.duplicate_selected").c_str(), ImVec2(buttonWidth, 0))) {
			this->createNewElementFrom(this->currentElement);
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("list_window.delete_selected").c_str(), ImVec2(buttonWidth, 0))) {
			this->deleteElement(this->currentElement);
		}
		ImGui::EndDisabled();

		if (this->currentElement != -1)
			renamePopup(this->getElementName(currentElement));

		drawListWindow();
	}
	const auto listWindowSize = ImGui::GetWindowSize();
	ImGui::End();

	listOverlays.emplace_back(local.get("info_overlay.list_window"));

	return listWindowSize;
}

void Windows::ListWindow::onButtonCreateNewElement() {
	this->createNewElement();
	this->currentElement = this->getListSize() - 1;
	ImGui::OpenPopup("##renameElementPopup");
}

void Windows::ListWindow::selectElement(int i) {
	this->currentElement = i;
}

int Windows::ListWindow::getElement() {
	return this->currentElement;
}

void Windows::ListWindow::drawOverlay() const {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		for (const auto &overlayText : this->listOverlays) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(overlayText.c_str());
			ImGui::PopTextWrapPos();
		}
	}
	ImGui::End();
}

void Windows::ListWindow::draw() {
	this->listOverlays.clear();

	this->currentElement = std::min(this->currentElement, getListSize() - 1);
	const ImVec2 listSize = drawList();
	if (this->currentElement != -1) {
		const auto displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(displaySize.x - listSize.x, 0), ImGuiCond_Appearing,
		                        ImVec2(1.0f, 0.0f));
		if (ImGui::Begin(this->getNameOption().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			drawOptions();
		}
		ImGui::End();
	}

	if (!this->listOverlays.empty())
		drawOverlay();
}

void Windows::ListWindow::unselect() {
	selectElement(-1);
}
