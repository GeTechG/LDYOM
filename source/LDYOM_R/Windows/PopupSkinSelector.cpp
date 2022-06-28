#include "PopupSkinSelector.h"

#include <CPed.h>

#include "imgui.h"
#include "fa.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

#include <utility>

#include "ModelsService.h"
#include "Settings.h"
#include "utils.h"
#include "CPopulation.h"
#include "CModelInfo.h"
#include "CPedModelInfo.h"

std::optional<ModelRenderer> PopupSkinSelector::renderer_{};

void PopupSkinSelector::clearUnknownSkins() {
	for (const auto & pair : this->unknownSkins_) {
		RwTextureDestroy(pair.second);
	}
	this->unknownSkins_.clear();
}

std::pair<IDirect3DTexture9*, ImVec2> PopupSkinSelector::getModelIcon(int modelId, bool special) {
	if (special && this->specialPedIcons_.contains(modelId)) {
		auto texture = this->specialPedIcons_.at(modelId).get();
		return {
			texture->getTexture(),
			ImVec2(texture->getWidth(), texture->getHeight())
		};
	}
	if (!special && this->pedsIcons_.contains(modelId)) {
		auto texture = this->pedsIcons_.at(modelId).get();
		return {
			texture->getTexture(),
			ImVec2(texture->getWidth(), texture->getHeight())
		};
	}
	if (!renderer_.has_value()) {
		renderer_ = ModelRenderer(CVector(0.f, 0, 0.f), 200, 200);
		renderer_.value().init();
		renderer_.value().getZoomKoef() = 5;
	}
	if (this->unknownSkins_.contains(modelId)) {
		if (this->unknownSkins_.at(modelId) != nullptr)
			return {
				*reinterpret_cast<IDirect3DTexture9**>(this->unknownSkins_.at(modelId)->raster + 1),
				ImVec2(200, 200)
			};
	} else {
		const auto pair = this->unknownSkins_.emplace(modelId, nullptr);
		if (special)
			renderer_.value().renderSpecialPed(ModelsService::getInstance().getSpecialsPed().at(modelId), &pair.first->second);
		else
			renderer_.value().render(modelId, &pair.first->second);
	}

	return {
		this->unknownIcon_->getTexture(),
		ImVec2(this->unknownIcon_->getWidth(), this->unknownIcon_->getHeight())
	};
}

void PopupSkinSelector::Init() {
	for (int model : ModelsService::getInstance().getPedModels()) {
		int imageWidth = 0;
		int imageHeight = 0;
		PDIRECT3DTEXTURE9 texture = nullptr;
		if (utils::LoadTextureFromFile(fmt::format("LDYOM/Resources/skinsIcons/Skinid{}.jpg", model).c_str(), &texture, &imageWidth, &imageHeight)) {
			this->pedsIcons_.emplace(model, std::make_unique<Texture>(texture, imageWidth, imageHeight));
		}
	}
	for (int i = 0; i < static_cast<int>(ModelsService::getInstance().getSpecialsPed().size()); ++i) {
		const auto & specialPed = ModelsService::getInstance().getSpecialsPed().at(i);
		int imageWidth = 0;
		int imageHeight = 0;
		PDIRECT3DTEXTURE9 texture = nullptr;
		if (utils::LoadTextureFromFile(fmt::format("LDYOM/Resources/specialsPeds/{}.jpg", specialPed).c_str(), &texture, &imageWidth, &imageHeight)) {
			this->specialPedIcons_.emplace(i, std::make_unique<Texture>(texture, imageWidth, imageHeight));
		}
	}
	auto unknownTextureIcon = utils::LoadTextureRequiredFromFile(L"LDYOM/Resources/skinsIcons/SkinNoIcon.jpg");
	if (unknownTextureIcon.has_value())
		this->unknownIcon_ = std::move(unknownTextureIcon.value());
	else
		throw std::exception("invalid load unknown texture icon");
}

void PopupSkinSelector::showPopup() {
	const auto popupName = fmt::format("{} {}", ICON_FA_TSHIRT, Localization::getInstance().get("skin_selector.title"));
	isOpen_ = true;
	this->clearUnknownSkins();
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

		if (ImGui::MenuItem(Localization::getInstance().get("filter_ped.male").c_str(), nullptr, filterPed.male))
			filterPed.male = !filterPed.male;
		if (ImGui::MenuItem(Localization::getInstance().get("filter_ped.female").c_str(), nullptr, filterPed.female))
			filterPed.female ^= TRUE;
		if (ImGui::MenuItem(Localization::getInstance().get("filter_ped.emergency").c_str(), nullptr, filterPed.emergency))
			filterPed.emergency ^= TRUE;
		if (ImGui::MenuItem(Localization::getInstance().get("filter_ped.cop").c_str(), nullptr, filterPed.cop))
			filterPed.cop ^= TRUE;

		ImGui::EndPopup();
	}
}

bool PopupSkinSelector::filteringPed(int model) const {
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

std::vector<int> getSpecialsModels() {
	std::vector<int> models;
	models.reserve(static_cast<int>(ModelsService::getInstance().getSpecialsPed().size()));
	for (int i = 0; i < static_cast<int>(ModelsService::getInstance().getSpecialsPed().size()); ++i) {
		models.emplace_back(i);
	}
	return models;
}

void PopupSkinSelector::renderPopup(const std::function<void(int)>& onSelectCallback, bool special, int slot) {
	static const std::vector<int> specialsPeds = getSpecialsModels();

	const std::vector<int> *models;

	if (special) {
		models = &specialsPeds;
	}
	else {
		models = &ModelsService::getInstance().getPedModels();
	}

	static char searchBuffer[256] = "";

	const auto &displaySize = ImGui::GetIO().DisplaySize;
	const auto popupName = fmt::format("{} {}", ICON_FA_TSHIRT, Localization::getInstance().get("skin_selector.title"));
	if (ImGui::BeginPopupModal(popupName.c_str(), &this->isOpen_, ImGuiWindowFlags_NoCollapse)) {

		/*ImGui::SetNextItemWidth(120);
		ImGui::InputText(Localization::getInstance().get("skin_selector.search").c_str(), searchBuffer, IM_ARRAYSIZE(searchBuffer));
		ImGui::SameLine();*/
		ImGui::SetNextItemWidth(120);
		if (ImGui::Button(Localization::getInstance().get("skin_selector.filter").c_str())) {
			ImGui::OpenPopup("filter");
		}

		filterPopup();

		if (ImGui::BeginChild("##skinsButtons", ImVec2(0.0f, 0.0f), true)) {
			int filled = 0;
			for (int i = 0; i < static_cast<int>(models->size()); ++i) {
				int model = models->at(i);

				if (!special) {
					if (filteringPed(model))
						continue;
				} else {
					if (filteringPed(290 + slot))
						continue;
				}

				ImGui::PushID(i);

				auto icon = getModelIcon(model, special);

				if (static_cast<float>(filled) + icon.second.x < ImGui::GetWindowContentRegionWidth()) {
					if (i > 0)
						ImGui::SameLine();
				} else {
					filled = 0;
				}

				if (ImGui::ImageButton(icon.first, icon.second)) {
					onSelectCallback(model);
					ImGui::CloseCurrentPopup();
				}
				filled += static_cast<int>(ImGui::GetItemRectSize().x);

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}
