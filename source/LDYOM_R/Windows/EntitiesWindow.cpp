#include "EntitiesWindow.h"

#include <fmt/core.h>
#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"

#include "ActorsWindow.h"
#include "AudioWindow.h"
#include "CheckpointsWindow.h"
#include "ObjectsWindow.h"
#include "ParticlesWindow.h"
#include "PickupsWindow.h"
#include "PyrotechnicsWindow.h"
#include "TrainsWindow.h"
#include "VehiclesWindow.h"
#include "VisualEffectsWindow.h"
#include "WindowsRenderService.h"

void Windows::EntitiesWindow::draw() {
	static auto &local = Localization::getInstance();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_SHAPES, local.get("entities.title")).c_str(), nullptr,
	                 ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_RUNNING, local.get("entities.actors")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ActorsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_CARS, local.get("entities.vehicles")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<VehiclesWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_CUBES, local.get("entities.objects")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ObjectsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_SPARKLES, local.get("entities.particles")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<ParticlesWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_TRAIN, local.get("entities.trains")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<TrainsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_HEART, local.get("entities.pickups")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<PickupsWindow>(true);
		}
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_FIRE_ALT, local.get("entities.pyrotechnics")).c_str(),
		                  ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<PyrotechnicsWindow>(true);
		}
		if (ImGui::Button(
			fmt::format("{} {}", ICON_FA_VOLUME, Localization::getInstance().get("entities.audio")).c_str(),
			ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<AudioWindow>(true);
		}
		if (ImGui::Button(
			fmt::format("{} {}", ICON_FA_SPARKLES, Localization::getInstance().get("entities.visual_effects")).c_str(),
			ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<VisualEffectsWindow>(true);
		}
		if (ImGui::Button(
			fmt::format("{} {}", ICON_FA_MAP_MARKER_ALT, Localization::getInstance().get("entities.checkpoints")).
			c_str(), ImVec2(ImGui::GetFontSize() * 12.5f, .0f))) {
			WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(false);
			WindowsRenderService::getInstance().toggleWindow<CheckpointsWindow>(true);
		}
	}
	ImGui::End();
}
