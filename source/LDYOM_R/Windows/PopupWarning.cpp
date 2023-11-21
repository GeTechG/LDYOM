#include "PopupWarning.h"

#include "imgui.h"
#include "Localization/Localization.h"

PopupWarning::PopupWarning(std::string name) : name(std::move(name)) {}

std::string& PopupWarning::getName() {
	return const_cast<std::string&>(Localization::getInstance().get(name));
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PopupWarning::draw(const std::function<void()> &onDelete, const std::function<void()> &onCancel) {
	auto &local = Localization::getInstance();
	if (ImGui::BeginPopupModal(getName().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(local.get("warning_popup.text").c_str());
		const float width = ImGui::GetContentRegionAvail().x / 2.0f - 3.0f;
		if (ImGui::Button(local.get("warning_popup.yes").c_str(), ImVec2(width, 0.0f))) {
			onDelete();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("warning_popup.no").c_str(), ImVec2(width, 0.0f))) {
			onCancel();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
