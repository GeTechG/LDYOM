#include "PopupWeaponSelector.h"

#include "CWeaponInfo.h"
#include "imgui.h"
#include "ModelsService.h"
#include "utils.h"
#include "fmt/core.h"

void PopupWeaponSelector::Init() {
	const auto &models = ModelsService::getInstance().getWeaponIds();
	for (int model : models) {
		int imageWidth = 0;
		int imageHeight = 0;
		PDIRECT3DTEXTURE9 texture = nullptr;
		if (utils::LoadTextureFromFile(fmt::format("LDYOM/Resources/weaponsIcons/50px-Weapon-{}-hd.png", model).c_str(),
		                               &texture, &imageWidth, &imageHeight)) {
			this->icons_.emplace(model, std::make_unique<Texture>(texture, imageWidth, imageHeight));
		}
	}
	auto unknownTextureIcon =
		utils::LoadTextureRequiredFromFile(L"LDYOM/Resources/weaponsIcons/50px-Weapon-unk-hd.png");
	if (unknownTextureIcon.has_value())
		this->unknownIcon_ = std::move(unknownTextureIcon.value());
	else
		throw std::exception("invalid load unknown texture icon");
}

void PopupWeaponSelector::showPopup() const {
	ImGui::OpenPopup("##weaponSelector");
}

void PopupWeaponSelector::weaponButton(const int *value) {
	const auto itr = this->icons_.find(*value);
	Texture *icon = itr == this->icons_.cend() ? this->unknownIcon_.get() : itr->second.get();

	const auto scale = ImGui::GetFontSize() / 16;
	if (ImGui::ImageButton(icon->getTexture(),
	                       ImVec2(static_cast<float>(icon->getWidth()) * scale,
	                              static_cast<float>(icon->getHeight()) * scale))) {
		showPopup();
	}
}

void PopupWeaponSelector::renderPopup(const std::function<void(int)> &onSelectCallback) {
	auto scale = ImGui::GetFontSize() / 16;
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(8 * (50 + 8)) * scale, 200.0f * scale));
	if (ImGui::BeginPopup("##weaponSelector")) {
		if (ImGui::BeginChild("##weaponsContainer")) {
			for (int i = 0; i < static_cast<int>(ModelsService::getInstance().getWeaponIds().size()); ++i) {
				int weaponId = ModelsService::getInstance().getWeaponIds().at(i);

				const auto itr = this->icons_.find(weaponId);
				Texture *icon = itr == this->icons_.cend() ? this->unknownIcon_.get() : itr->second.get();

				if (i % 8 != 0) {
					ImGui::SameLine();
				}

				if (ImGui::ImageButton(icon->getTexture(),
				                       ImVec2(static_cast<float>(icon->getWidth()) * scale,
				                              static_cast<float>(icon->getHeight()) * scale))) {
					onSelectCallback(weaponId);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(CWeaponInfo::ms_aWeaponNames[weaponId]);
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}
