#include "PopupWarning.h"

#include "imgui.h"
#include "Localization/Localization.h"

PopupWarning::PopupWarning(std::string name) : name(std::move(name)) {}

char* PopupWarning::getName() {
	return const_cast<char*>(Localization::getInstance().get(name).c_str());
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PopupWarning::draw(const std::function<void()>& onDelete) {
	auto &local = Localization::getInstance();
	if (ImGui::BeginPopupModal(getName(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text(local.get("warning_popup.text").c_str());
		const float width = ImGui::GetContentRegionAvail().x / 2.0f - 3.0f;
		if (ImGui::Button(local.get("warning_popup.yes").c_str(), ImVec2(width, 0.0f))) {
			onDelete();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("warning_popup.no").c_str(), ImVec2(width, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
