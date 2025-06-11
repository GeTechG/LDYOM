#include "weapon_selector.h"
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <models_manager.h>
#include <textures_manager.h>
#include <fmt/format.h>

std::optional<ModelRenderer> PopupWeaponSelector::m_modelRenderer = std::nullopt;

Texture* PopupWeaponSelector::getWeaponIcon(int weaponId) {
	if (!m_modelRenderer.has_value()) {
		m_modelRenderer = ModelRenderer(CVector(0.f, 0.f, -0.2f), 50, 50);
		m_modelRenderer.value().init();
		m_modelRenderer.value().getZoomKoef() = 0.1f;
	}

	const auto modelName = fmt::format("50px-Weapon-{}-hd", weaponId);
	if (TexturesManager::instance().hasTexture(modelName)) {
		return TexturesManager::instance().getTexture(modelName).value();
	}

	TexturesManager::instance().addTexture(modelName, std::make_unique<RenderwareTexture>());
	auto newTexture = TexturesManager::instance().getTexture(modelName).value();
	auto weaponInfo = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weaponId), 1);
	auto weaponModel = weaponInfo->m_nModelId;
	m_modelRenderer.value().render(weaponModel, dynamic_cast<RenderwareTexture*>(newTexture));

	return newTexture;
}

void PopupWeaponSelector::showPopup(std::string_view title) {
	const auto popupName = _("weapon_selector.title", ICON_FA_GUN) + "##" + std::string(title);
	ImGui::OpenPopup(popupName.c_str());
}

void PopupWeaponSelector::renderPopup(std::string_view title, bool* open,
                                      const std::function<void(int)>& onSelectCallback) {
	static float scale = 1.0f;

	const auto& displaySize = ImGui::GetIO().DisplaySize;
	const auto popupName = _("weapon_selector.title", ICON_FA_GUN) + "##" + std::string(title);
	ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f), ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal(popupName.c_str(), open, ImGuiWindowFlags_NoCollapse)) {
		ImGui::DragFloat("##scale", &scale, 0.01f, 0.1f, NULL, "%.2f");

		if (ImGui::BeginChild("##weaponsButtons", ImVec2(0.0f, 0.0f), true)) {
			const int modelsCount = ModelsManager::getWeaponIds().size();

			const float availableWidthInWindow =
				ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			const float contentMinScreenX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;

			for (int i = 0; i < modelsCount; ++i) {
				ImGui::PushID(i);

				auto icon = getWeaponIcon(ModelsManager::getWeaponIds()[i]);

				if (icon->getTexture() == nullptr) {
					icon = TexturesManager::instance().getTexture("50px-Weapon-unk-hd").value();
				}

				auto size = ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight()));
				size.x *= scale * 2;
				size.y *= scale * 2;

				if (i > 0) {
					float lastItemMaxScreenX = ImGui::GetItemRectMax().x;
					float currentLineWidth = lastItemMaxScreenX - contentMinScreenX;

					if (currentLineWidth + ImGui::GetStyle().ItemSpacing.x + size.x < availableWidthInWindow) {
						ImGui::SameLine();
					}
				}

				if (ImGui::ImageButton("", (ImTextureID)icon->getTexture(), size)) {
					onSelectCallback(ModelsManager::getWeaponIds()[i]);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::IsItemHovered()) {
					auto weaponId = ModelsManager::getWeaponIds()[i];
					ImGui::SetTooltip("%s, ID: %d", CWeaponInfo::ms_aWeaponNames[weaponId], weaponId);
				}

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}