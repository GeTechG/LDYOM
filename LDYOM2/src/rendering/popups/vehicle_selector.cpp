#include "vehicle_selector.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <imgui.h>
#include <localization.h>
#include <models_manager.h>
#include <textures_manager.h>

bool PopupVehicleSelector::isOpen = false;
std::optional<ModelRenderer> PopupVehicleSelector::m_modelRenderer = std::nullopt;

void PopupVehicleSelector::showPopup() {
	const auto popupName = _("vehicle_selector.title", ICON_FA_CAR);
	isOpen = !isOpen;
	ImGui::OpenPopup(popupName.c_str());
}

struct FilterVehicle {
	bool filter_car = true;
	bool filter_bike = true;
	bool filter_mtrack = true;
	bool filter_quad = true;
	bool filter_plane = true;
	bool filter_heli = true;
	bool filter_bmx = true;
	bool filter_trailer = true;
	bool filter_boat = true;
} filterVehicle;

namespace {
void filterPopup() {
	if (ImGui::BeginPopup("filter")) {
		if (ImGui::MenuItem(_("vehicle_selector.car").c_str(), nullptr, filterVehicle.filter_car))
			filterVehicle.filter_car = !filterVehicle.filter_car;
		if (ImGui::MenuItem(_("vehicle_selector.bike").c_str(), nullptr, filterVehicle.filter_bike))
			filterVehicle.filter_bike ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.mtrack").c_str(), nullptr, filterVehicle.filter_mtrack))
			filterVehicle.filter_mtrack ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.quad").c_str(), nullptr, filterVehicle.filter_quad))
			filterVehicle.filter_quad ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.plane").c_str(), nullptr, filterVehicle.filter_plane))
			filterVehicle.filter_plane ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.heli").c_str(), nullptr, filterVehicle.filter_heli))
			filterVehicle.filter_heli ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.bmx").c_str(), nullptr, filterVehicle.filter_bmx))
			filterVehicle.filter_bmx ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.trailer").c_str(), nullptr, filterVehicle.filter_trailer))
			filterVehicle.filter_trailer ^= true;
		if (ImGui::MenuItem(_("vehicle_selector.boat").c_str(), nullptr, filterVehicle.filter_boat))
			filterVehicle.filter_boat ^= true;

		ImGui::EndPopup();
	}
}
} // namespace

bool PopupVehicleSelector::filteringVehicle(int model) {
	const auto vehicleType = reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(model))->m_nVehicleType;

	switch (vehicleType) {
		case VEHICLE_MTRUCK: return !filterVehicle.filter_mtrack;
		case VEHICLE_QUAD: return !filterVehicle.filter_quad;
		case VEHICLE_HELI: return !filterVehicle.filter_heli;
		case VEHICLE_PLANE: return !filterVehicle.filter_plane;
		case VEHICLE_BIKE: return !filterVehicle.filter_bike;
		case VEHICLE_BMX: return !filterVehicle.filter_bmx;
		case VEHICLE_TRAILER: return !filterVehicle.filter_trailer;
		case VEHICLE_BOAT: return !filterVehicle.filter_boat;
		case VEHICLE_TRAIN: return true;
		default: return !filterVehicle.filter_car;
	}
}

Texture* PopupVehicleSelector::getModelIcon(int model) {
	const auto modelName = fmt::format("Vehicle_{}", model);
	if (TexturesManager::instance().hasTexture(modelName)) {
		return TexturesManager::instance().getTexture(modelName).value();
	}

	if (!m_modelRenderer.has_value()) {
		m_modelRenderer = ModelRenderer(CVector(0.f, 0, 0.f), 204, 125);
		m_modelRenderer.value().init();
		m_modelRenderer.value().getZoomKoef() = 5;
	}

	TexturesManager::instance().addTexture(modelName, std::make_unique<RenderwareTexture>());
	auto newTexture = TexturesManager::instance().getTexture(modelName).value();
	m_modelRenderer.value().render(model, dynamic_cast<RenderwareTexture*>(newTexture));

	return newTexture;
}

void PopupVehicleSelector::renderPopup(const std::function<void(int)>& onSelectCallback) {
	// static char searchBuffer[256] = "";
	static float scale = 1.0f;

	const auto& displaySize = ImGui::GetIO().DisplaySize;
	const auto popupName = _("vehicle_selector.title", ICON_FA_CAR);
	ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.5f, displaySize.y * 0.5f), ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal(popupName.c_str(), &isOpen, ImGuiWindowFlags_NoCollapse)) {
		/*ImGui::SetNextItemWidth(120);
		ImGui::InputText(Localization::getInstance().get("vehicle_selector.search").c_str(), searchBuffer,
		IM_ARRAYSIZE(searchBuffer)); ImGui::SameLine();*/

		if (ImGui::Button(_("vehicle_selector.filter").c_str())) {
			ImGui::OpenPopup("filter");
		}
		filterPopup();
		ImGui::SameLine();

		ImGui::DragFloat("##scale", &scale, 0.01f, 0.1f, NULL, "%.2f");

		if (ImGui::BeginChild("##skinsButtons", ImVec2(0.0f, 0.0f), true)) {
			const int modelsCount = ModelsManager::getVehicleModels().size();

			const float availableWidthInWindow =
				ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
			const float contentMinScreenX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;

			for (int i = 0; i < modelsCount; ++i) {
				const auto model = ModelsManager::getVehicleModels()[i];
				if (filteringVehicle(model))
					continue;

				ImGui::PushID(i);

				auto icon = getModelIcon(model);

				if (icon->getTexture() == nullptr) {
					icon = TexturesManager::instance().getTexture("Vehicle_unk").value();
				}

				auto size = ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight()));
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
					onSelectCallback(model);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("ID: %d", model);
				}

				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}