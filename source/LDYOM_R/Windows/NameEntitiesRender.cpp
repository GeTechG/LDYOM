#include "NameEntitiesRender.h"

#include <CWorld.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "CSprite.h"
#include "imgui.h"
#include "ProjectsService.h"
#include "Settings.h"

void drawEntityName(const float *position, const float zOffset, const char *name) {
	const auto drawList = ImGui::GetBackgroundDrawList();

	const float distance = DistanceBetweenPoints(CVector(position[0], position[1], position[2]),
	                                             FindPlayerPed()->GetPosition());
	if (distance < Settings::getInstance().get<float>("main.distanceShowNames").value_or(100.0f)) {
		RwV3d screenCoors;
		float w, h;
		if (CSprite::CalcScreenCoors(RwV3d{position[0], position[1], position[2] + zOffset}, &screenCoors, &w, &h, true,
		                             true)) {
			const auto textSize = ImGui::CalcTextSize(name);
			const auto textPos = ImVec2(screenCoors.x - textSize.x / 2.0f, screenCoors.y - textSize.y / 2.0f);
			drawList->AddRectFilled(textPos, textPos + textSize,
			                        ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.5f)));
			drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(ImColor(255, 255, 255).Value), name);
		}
	}
}

void NameEntitiesRender::draw() {
	auto entitiesNamesRender = []<typename T>(std::vector<std::unique_ptr<T>> &entities, float zOffset = 1.f) {
		for (const auto &entity : entities) {
			const auto position = entity->getPosition();
			drawEntityName(position, zOffset, entity->getName());
		}
	};

	const auto currentScene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();

	for (const auto &objective : currentScene->getObjectives()) {
		if (auto *positionable = dynamic_cast<IPositionable*>(objective.get())) {
			const auto position = positionable->getPosition();
			drawEntityName(position, 1.f, objective->getName());
		}
	}

	entitiesNamesRender(currentScene->getActors());
	entitiesNamesRender(currentScene->getVehicles(), 2.f);
	entitiesNamesRender(currentScene->getObjects());
	entitiesNamesRender(currentScene->getParticles());
	entitiesNamesRender(currentScene->getTrains(), 2.f);
	entitiesNamesRender(currentScene->getPickups());
	entitiesNamesRender(currentScene->getPyrotechnics());
	entitiesNamesRender(currentScene->getAudio());
	entitiesNamesRender(currentScene->getVisualEffects());
	entitiesNamesRender(currentScene->getCheckpoints());
}
