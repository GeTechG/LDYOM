#include "NameEntitiesRender.h"

#include <CWorld.h>

#include "imgui.h"
#include "CSprite.h"
#include "ProjectsService.h"
#include "Settings.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

void drawEntityName(const float* position, const float zOffset, const char* name) {
	const auto drawList = ImGui::GetBackgroundDrawList();

	const float distance = DistanceBetweenPoints(CVector(position[0], position[1], position[2]), FindPlayerPed()->GetPosition());
	if (distance < Settings::getInstance().get<float>("main.distanceShowNames").value_or(100.0f)) {
		RwV3d screenCoors; float w, h;
		if (CSprite::CalcScreenCoors(RwV3d{ position[0], position[1], position[2] + zOffset }, &screenCoors, &w, &h, true, true)) {
			const auto textSize = ImGui::CalcTextSize(name);
			const auto textPos = ImVec2(screenCoors.x - textSize.x / 2.0f, screenCoors.y - textSize.y / 2.0f);
			drawList->AddRectFilled(textPos, textPos + textSize, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.5f)));
			drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(ImColor(255, 255, 255).Value), name);
		}
	}
}

void NameEntitiesRender::draw() {

	for (const auto & actor : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors()) {
		const auto position = actor->getPosition();
		drawEntityName(position, 1.0f, actor->getName());
	}
	for (const auto& vehicle : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles()) {
		const auto position = vehicle->getPosition();
		drawEntityName(position, 2.0f, vehicle->getName());
	}
	for (const auto& objective : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives()) {
		if (auto* positionable = dynamic_cast<IPositionable*>(objective.get())) {
			const auto position = positionable->getPosition();
			drawEntityName(position, 0.0f, objective->getName());
		}
	}
}
