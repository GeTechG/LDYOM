#include "PopupVehicleSelector.h"

#include <CModelInfo.h>

#include "imgui.h"
#include "ModelsService.h"
#include "utils.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

PopupVehicleSelector::PopupVehicleSelector() {
	for (int model : ModelsService::getInstance().getVehicleModels()) {
		int imageWidth = 0;
		int imageHeight = 0;
		PDIRECT3DTEXTURE9 texture = nullptr;
		if (utils::LoadTextureFromFile(fmt::format("LDYOM/Resources/vehiclesIcons/Vehicle_{}.jpg", model).c_str(), &texture, &imageWidth, &imageHeight)) {
			this->vehicleIcons_.emplace(model, std::make_unique<Texture>(texture, imageWidth, imageHeight));
		}
	}
	auto unknownTextureIcon = utils::LoadTextureRequiredFromFile(L"LDYOM/Resources/vehiclesIcons/Vehicle_unk.jpg");
	if (unknownTextureIcon.has_value())
		this->unknownIcon_ = std::move(unknownTextureIcon.value());
	else
		throw std::exception("invalid load unknown texture icon");
}

void PopupVehicleSelector::show() {
	this->bShow = true;
	ImGui::OpenPopup("##carSelector");
}

void PopupVehicleSelector::draw(const std::function<void(int model)> onSelect) {
	auto &local = Localization::getInstance();
	ImGui::SetNextWindowSize(ImVec2(850,580), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("##carSelector", &this->bShow)) {

		ImGui::SetNextItemWidth(120);
		utils::Combo("##searchType", &typeSearch, local.getArray("vehicle_selector.searchTypeEnum"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::InputText(local.get("vehicle_selector.searchType").c_str(), search, IM_ARRAYSIZE(search));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		if (ImGui::Button(local.get("vehicle_selector.filters").c_str())) {
			ImGui::OpenPopup("filters");
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::DragFloat(local.get("vehicle_selector.icons_scale").c_str(), &scale, 0.01f, 0.0001f, 100.0f);

		if (ImGui::BeginPopup("filters")) {

			const auto filtersEnum = local.getArray("vehicle_selector.filtersEnum");
			if (ImGui::MenuItem(filtersEnum[0].c_str(), "", filter_car))
				filter_car = !filter_car;
			if (ImGui::MenuItem(filtersEnum[1].c_str(), "", filter_bike))
				filter_bike = !filter_bike;
			if (ImGui::MenuItem(filtersEnum[2].c_str(), "", filter_mtrack))
				filter_mtrack = !filter_mtrack;
			if (ImGui::MenuItem(filtersEnum[3].c_str(), "", filter_quad))
				filter_quad = !filter_quad;
			if (ImGui::MenuItem(filtersEnum[4].c_str(), "", filter_heli))
				filter_heli = !filter_heli;
			if (ImGui::MenuItem(filtersEnum[5].c_str(), "", filter_plane))
				filter_plane = !filter_plane;
			if (ImGui::MenuItem(filtersEnum[6].c_str(), "", filter_bmx))
				filter_bmx = !filter_bmx;
			if (ImGui::MenuItem(filtersEnum[7].c_str(), "", filter_trailer))
				filter_trailer = !filter_trailer;
			if (ImGui::MenuItem(filtersEnum[8].c_str(), "", filter_boat))
				filter_boat = !filter_boat;

			ImGui::EndPopup();
		}

		ImGui::BeginChild("vehs", ImVec2(ImGui::GetWindowWidth() - 5.0f, ImGui::GetWindowHeight() - ImGui::GetCursorPosY() - 5), true);

		float width = 0.0f;
		for (int i = 400; i <= 611; i++) {
			const auto & name = &reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(i))->m_szGameName[0];

			if (search[0] != '\0') {
				if (typeSearch == 0) {
					if (std::string(name).find(search) == std::string::npos)
						continue;
				}
				else {
					if (std::to_string(i).find(search) == std::string::npos)
						continue;
				}
			}
			switch (reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(i))->m_nVehicleType) {
			case VEHICLE_MTRUCK:
				if (!filter_mtrack) {
					continue;
				}
				break;
			case VEHICLE_QUAD:
				if (!filter_quad) {
					continue;
				}
				break;
			case VEHICLE_HELI:
				if (!filter_heli) {
					continue;
				}
				break;
			case VEHICLE_PLANE:
				if (!filter_plane) {
					continue;
				}
				break;
			case VEHICLE_BIKE:
				if (!filter_bike) {
					continue;
				}
				break;
			case VEHICLE_BMX:
				if (!filter_bmx) {
					continue;
				}
				break;
			case VEHICLE_TRAILER:
				if (!filter_trailer) {
					continue;
				}
				break;
			case VEHICLE_BOAT:
				if (!filter_boat) {
					continue;
				}
				break;
			case VEHICLE_TRAIN:
				continue;
			default:
				if (!filter_car) {
					continue;
				}
				break;
			}

			const auto itr = vehicleIcons_.find(i);
			Texture* icon = itr == vehicleIcons_.cend() ? this->unknownIcon_.get() : itr->second.get();

			const float widthNextIcon = static_cast<float>(icon->getWidth()) * scale;

			if (width + widthNextIcon < ImGui::GetWindowSize().x) {
				ImGui::SameLine();
			} else {
				width = 0.0f;
			}

			if (ImGui::ImageButton(icon->getTexture(), ImVec2(
				widthNextIcon, 
				static_cast<float>(icon->getHeight()) * scale))) {
				onSelect(i);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::IsItemHovered()) {
				auto info = fmt::format("ModelID: {}\nName: {}", i, name);
				ImGui::SetTooltip(info.c_str());
			}

			width += widthNextIcon;
		}

		ImGui::EndChild();

		ImGui::EndPopup();
	}
}
