#pragma once
#include "components/vehicle.h"
#include <entities_manager.h>
#include <format>


template <typename T>
inline void renderEntityByComponentSelection(std::string& entityId, float availableWidth, float labelWidth) {
	ImGui::Text(_(fmt::format("components.{}.name", T::TYPE)).c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);

	const auto entities = EntitiesManager::instance().getEntitiesWithComponent(T::TYPE);
	const auto entityUuid = uuids::uuid::from_string(entityId);
	const auto currentEntity =
		std::ranges::find_if(entities, [entityUuid](Entity* entity) { return entity->id == entityUuid; });

	const char* previewText = (currentEntity != entities.end()) ? (*currentEntity)->name.c_str() : "";

	if (ImGui::BeginCombo("##entityId", previewText)) {
		for (size_t i = 0; i < entities.size(); ++i) {
			const auto& entity = entities[i];
			const bool isSelected = (entity->id == entityUuid);

			if (ImGui::Selectable(fmt::format("{}##{}", entity->name, i).c_str(), isSelected)) {
				entityId = to_string(entity->id);
			}
		}
		ImGui::EndCombo();
	}
}