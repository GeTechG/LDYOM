#include "ListWindow.h"
#include "imgui.h"
#include "constants.h"
#include "Localization/Localization.h"

void renamePopup(char *name) {
	if (ImGui::BeginPopup("##renameElementPopup")) {
		ImGui::InputText("##inputNameRename", name, NAME_SIZE, ImGuiInputTextFlags_EnterReturnsTrue);
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Windows::ListWindow::drawList() {
	static auto& local = Localization::getInstance();

	const auto windowSize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos(ImVec2(windowSize.x, 0.0f), ImGuiCond_Appearing, ImVec2(1.0f, 0.0f));
	if (ImGui::Begin(this->getNameList().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		const auto listSize = ImVec2(windowSize.x / 4.0f, windowSize.y / 2.0f);

		if (ImGui::BeginChild("##ListElements", listSize, true)) {
			for (int i = 0; i < this->getListSize(); ++i) {
				ImGui::PushID(i);
				if (ImGui::Selectable(this->getElementName(i), this->currentElement == i)) {
					this->currentElement = i;
					selectElement(i);
				}

				if (this->dragsItems_) {
					if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
						const int nextItem = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
						if (nextItem >= 0 && nextItem < this->getListSize()) {
							this->swapItems(i, nextItem);
							ImGui::ResetMouseDragDelta();
						}
					}
				}

				if (ImGui::BeginPopupContextItem("##editElementName")) {
					ImGui::InputText("##inputNameRename", this->getElementName(i), NAME_SIZE, ImGuiInputTextFlags_EnterReturnsTrue);
					if (ImGui::IsItemDeactivatedAfterEdit()) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
			ImGui::EndChild();
		}

		if (ImGui::Button(local.get("list_window.create_new").c_str(), ImVec2(listSize.x / 2.0f - 3.0f, 0))) {
			onButtonCreateNewElement();
		}
		ImGui::SameLine();
		ImGui::BeginDisabled(this->currentElement == -1);
		if (ImGui::Button(local.get("list_window.delete_selected").c_str(), ImVec2(listSize.x / 2.0f - 3.0f, 0))) {
			this->deleteElement(this->currentElement);
		}
		ImGui::EndDisabled();

		if (this->currentElement != -1)
			renamePopup(this->getElementName(currentElement));

		drawListWindow();
	}
	ImGui::End();
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

void Windows::ListWindow::draw() {
	this->currentElement = min(this->currentElement, getListSize() - 1);
	drawList();
	if (this->currentElement != -1) {
		const auto windowSize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(windowSize.x - ImGui::GetWindowWidth(), 0.0f), ImGuiCond_Appearing, ImVec2(1.0f, 0.0f));
		if (ImGui::Begin(this->getNameOption().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			drawOptions();
		}
		ImGui::End();
	}
}

void Windows::ListWindow::unselect() {
	this->currentElement = -1;
}
