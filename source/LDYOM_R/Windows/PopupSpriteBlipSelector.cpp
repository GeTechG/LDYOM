#include "PopupSpriteBlipSelector.h"

#include <CRadar.h>

#include "imgui.h"
#include "utils.h"
#include "fmt/core.h"
#include "Localization/Localization.h"


void PopupSpriteBlipSelector::Init() {
	for (int i = 0; i < 64; ++i) {
		auto unknownTextureIcon = utils::LoadTextureRequiredFromFile(fmt::format("LDYOM/Resources/blipSprites/Radar_icon{}.png", i));
		if (unknownTextureIcon.has_value())
			blipIcons_.emplace(i, std::move(unknownTextureIcon.value()));
		else
			throw std::exception("invalid load unknown texture icon");
	}
}

void PopupSpriteBlipSelector::draw(int* blip, const std::function<void(int)> callback) {
	const float size = ImGui::GetFontSize() * 1.3f;
	if (ImGui::ImageButton(PopupSpriteBlipSelector::blipIcons_.at(*blip)->getTexture(), ImVec2(size, size))) {
		this->open_ = true;
		ImGui::OpenPopup("##selectBlip");
	}
	ImGui::SameLine();
	ImGui::Text(Localization::getInstance().get("general.icon").c_str());
	if (ImGui::BeginPopup("##selectBlip")) {
		for (int i = 0; i < 64; ++i) {
			if (i % 8 != 0) {
				ImGui::SameLine();
			}
			if (ImGui::ImageButton(PopupSpriteBlipSelector::blipIcons_.at(i)->getTexture(), ImVec2(size, size))) {
				*blip = i;
				callback(i);
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
}
