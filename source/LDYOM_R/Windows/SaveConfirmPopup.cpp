#include "SaveConfirmPopup.h"

#include "imgui.h"

void Windows::SaveConfirmPopup::draw() {
	auto& local = Localization::getInstance();
	if (ImGui::BeginPopupModal(local.get("save_objective.show_confirm").c_str(), &this->show_)) {

		ImGui::Text(local.get("save_objective.confirm_text").c_str());
		const float width = ImGui::GetContentRegionAvail().x / 2.0f - 3.0f;
		if (ImGui::Button(local.get("general.yes").c_str(), ImVec2(width, 0.0f))) {
			this->onSave();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("general.no").c_str(), ImVec2(width, 0.0f))) {
			this->onCancel();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

boost::signals2::signal<void()>& Windows::SaveConfirmPopup::getOnSave() {
	return onSave;
}

boost::signals2::signal<void()>& Windows::SaveConfirmPopup::getOnCancel() {
	return onCancel;
}
