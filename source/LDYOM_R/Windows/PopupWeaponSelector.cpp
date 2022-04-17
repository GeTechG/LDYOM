#include "PopupWeaponSelector.h"

#include "ModelsService.h"
#include "utils.h"
#include "fmt/core.h"
#include "imgui.h"

void PopupWeaponSelector::Init() {
	const auto& models = ModelsService::getInstance().getWeaponIds();
	for (int model : models) {
		int imageWidth = 0;
		int imageHeight = 0;
		PDIRECT3DTEXTURE9 texture = nullptr;
		if (utils::LoadTextureFromFile(fmt::format("LDYOM/Resources/weaponsIcons/50px-Weapon-{}-hd.png", model).c_str(), &texture, &imageWidth, &imageHeight)) {
			this->icons_.emplace(model, Texture(texture, imageWidth, imageHeight));
		}
	}
	const std::optional<Texture> unknownTextureIcon = utils::LoadTextureRequiredFromFile("LDYOM/Resources/weaponsIcons/50px-Weapon-unk-hd.png");
	if (unknownTextureIcon.has_value())
		this->unknownIcon_ = unknownTextureIcon;
	else
		throw std::exception("invalid load unknown texture icon");
}

void PopupWeaponSelector::showPopup() const {
	ImGui::OpenPopup("##weaponSelector");
}

void PopupWeaponSelector::weaponButton(const int* value) {
	const auto itr = this->icons_.find(*value);
	Texture* icon = itr == this->icons_.cend() ? &this->unknownIcon_.value() : &itr->second;

	if (ImGui::ImageButton(icon->getTexture(), ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight())))) {
		showPopup();
	}
}

void PopupWeaponSelector::renderPopup(const std::function<void(int)>& onSelectCallback) {
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(8 * (50 + 10)), 200.0f));
	if (ImGui::BeginPopup("##weaponSelector")) {

		if (ImGui::BeginChild("##weaponsContainer")) {
			for (int i = 0; i < static_cast<int>(ModelsService::getInstance().getWeaponIds().size()); ++i) {
				int weaponId = ModelsService::getInstance().getWeaponIds().at(i);

				const auto itr = this->icons_.find(weaponId);
				Texture* icon = itr == this->icons_.cend() ? &this->unknownIcon_.value() : &itr->second;

				if (i % 8 != 0) {
					ImGui::SameLine();
				}

				if (ImGui::ImageButton(icon->getTexture(), ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight())))) {
					onSelectCallback(weaponId);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}
