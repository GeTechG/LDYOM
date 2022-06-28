#include "EntitiesWindow.h"

#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"
#include <fmt/core.h>

#include "ActorsWindow.h"
#include "ObjectsWindow.h"
#include "ParticlesWindow.h"
#include "PickupWindow.h"
#include "TrainsWindow.h"
#include "VehiclesWindow.h"
#include "WindowsRenderService.h"

void Windows::EntitiesWindow::draw()
{
	static auto& local = Localization::getInstance();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_SHAPES, local.get("entities.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_RUNNING, local.get("entities.actors")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ActorsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_CARS, local.get("entities.vehicles")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<VehiclesWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_CUBES, local.get("entities.objects")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ObjectsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_SPARKLES, local.get("entities.particles")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ParticlesWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_TRAIN, local.get("entities.trains")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<TrainsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_HEART, local.get("entities.pickups")).c_str(), ImVec2(200.0f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<PickupWindow>(true);
		}
	}
	ImGui::End();
}
