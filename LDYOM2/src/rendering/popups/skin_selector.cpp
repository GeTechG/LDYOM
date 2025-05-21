#include "skin_selector.h"
#include <CPedModelInfo.h>
#include <CPopulation.h>
#include <fa_icons.h>
#include <fmt/format.h>
#include <imgui.h>
#include <localization.h>
#include <models_manager.h>
#include <textures_manager.h>

bool PopupSkinSelector::isOpen = false;
std::optional<ModelRenderer> PopupSkinSelector::m_modelRenderer = std::nullopt;

void PopupSkinSelector::showPopup() {
	const auto popupName = _("skin_selector.title", ICON_FA_SHIRT);
	isOpen = !isOpen;
	ImGui::OpenPopup(popupName.c_str());
}

struct FilterPed {
	bool female = true;
	bool male = true;
	bool cop = true;
	bool emergency = true;
} filterPed;

inline void filterPopup() {
	if (ImGui::BeginPopup("filter")) {
		if (ImGui::MenuItem(_("skin_selector.male").c_str(), nullptr, filterPed.male))
			filterPed.male = !filterPed.male;
		if (ImGui::MenuItem(_("skin_selector.female").c_str(), nullptr, filterPed.female))
			filterPed.female ^= true;
		if (ImGui::MenuItem(_("skin_selector.emergency").c_str(), nullptr, filterPed.emergency))
			filterPed.emergency ^= true;
		if (ImGui::MenuItem(_("skin_selector.cop").c_str(), nullptr, filterPed.cop))
			filterPed.cop ^= true;

		ImGui::EndPopup();
	}
}

bool PopupSkinSelector::filteringPed(int model) {
	const unsigned pedType = reinterpret_cast<CPedModelInfo*>(CModelInfo::GetModelInfo(model))->m_nPedType;
	if (pedType == 6 && !filterPed.cop)
		return true;
	if ((pedType == 18 || pedType == 19) && !filterPed.emergency)
		return true;
	if (CPopulation::IsMale(model) && !filterPed.male)
		return true;
	if (CPopulation::IsFemale(model) && !filterPed.female)
		return true;
	return false;
}

Texture* PopupSkinSelector::getModelIcon(Skin skin) {
	const auto modelName =
		skin.model == -1 ? fmt::format("special_{}", skin.special) : fmt::format("Skinid{}", skin.model);
	if (TexturesManager::instance().hasTexture(modelName)) {
		return TexturesManager::instance().getTexture(modelName).value();
	}

	if (!m_modelRenderer.has_value()) {
		m_modelRenderer = ModelRenderer(CVector(0.f, 0, 0.f), 100, 290);
		m_modelRenderer.value().init();
		m_modelRenderer.value().getZoomKoef() = 5.5;
	}

	TexturesManager::instance().addTexture(modelName, std::make_unique<RenderwareTexture>());
	auto newTexture = TexturesManager::instance().getTexture(modelName).value();
	if (skin.model == -1)
		m_modelRenderer.value().renderSpecialPed(skin.special, dynamic_cast<RenderwareTexture*>(newTexture));
	else
		m_modelRenderer.value().render(skin.model, dynamic_cast<RenderwareTexture*>(newTexture));

	return newTexture;
}

void PopupSkinSelector::renderPopup(const std::function<void(Skin)>& onSelectCallback, bool special) {
	// static char searchBuffer[256] = "";
	static float scale = 1.0f;

	const auto& displaySize = ImGui::GetIO().DisplaySize;
	const auto popupName = _("skin_selector.title", ICON_FA_SHIRT);
	ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f), ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal(popupName.c_str(), &isOpen, ImGuiWindowFlags_NoCollapse)) {
		/*ImGui::SetNextItemWidth(120);
		ImGui::InputText(Localization::getInstance().get("skin_selector.search").c_str(), searchBuffer,
		IM_ARRAYSIZE(searchBuffer)); ImGui::SameLine();*/

		if (!special) {
			if (ImGui::Button(_("skin_selector.filter").c_str())) {
				ImGui::OpenPopup("filter");
			}
			filterPopup();
			ImGui::SameLine();
		}

		ImGui::DragFloat("##scale", &scale, 0.01f, 0.1f, NULL, "%.2f");

		if (ImGui::BeginChild("##skinsButtons", ImVec2(0.0f, 0.0f), true)) {
			const int modelsCount =
				special ? ModelsManager::GetPedSpecialModels().size() : ModelsManager::GetPedModels().size();

			const float availableWidthInWindow =
				ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			const float contentMinScreenX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;

			for (int i = 0; i < modelsCount; ++i) {
				if (!special) {
					const auto model = ModelsManager::GetPedModels()[i];
					if (filteringPed(model))
						continue;
				}

				ImGui::PushID(i);

				auto icon = PopupSkinSelector::getModelIcon(Skin{
					.model = special ? -1 : ModelsManager::GetPedModels()[i],
					.special = special ? ModelsManager::GetPedSpecialModels()[i] : "",
				});

				if (icon->getTexture() == nullptr) {
					icon = TexturesManager::instance().getTexture("SkinNoIcon").value();
				}

				auto size = ImVec2((float)icon->getWidth(), (float)icon->getHeight());
				size.x *= scale;
				size.y *= scale;

				if (i > 0) {
					float lastItemMaxScreenX = ImGui::GetItemRectMax().x;
					float currentLineWidth = lastItemMaxScreenX - contentMinScreenX;

					if (currentLineWidth + ImGui::GetStyle().ItemSpacing.x + size.x < availableWidthInWindow) {
						ImGui::SameLine();
					}
				}

				if (ImGui::ImageButton("", (ImTextureID)icon->getTexture(), size)) {
					onSelectCallback(Skin{
						.model = special ? -1 : ModelsManager::GetPedModels()[i],
						.special = special ? ModelsManager::GetPedSpecialModels()[i] : "",
					});
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::IsItemHovered()) {
					if (special) {
						ImGui::SetTooltip("%s", ModelsManager::GetPedSpecialModels()[i].c_str());
					} else {
						ImGui::SetTooltip("ID: %d", ModelsManager::GetPedModels()[i]);
					}
				}

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}