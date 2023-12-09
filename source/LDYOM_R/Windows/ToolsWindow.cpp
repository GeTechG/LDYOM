#include "ToolsWindow.h"

#include <common.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>
#include <fmt/core.h>
#include "fa.h"
#include "imgui.h"
#include "plugin.h"
#include "ProjectsService.h"
#include "utils.h"
#include "Localization/Localization.h"

using namespace plugin;

void Windows::ToolsWindow::draw() {
	static auto &local = Localization::getInstance();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 30.f, ImGui::GetFontSize() * 10.f), ImGuiCond_Appearing);
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_TOOLS, local.get("tools.title")).c_str(), nullptr,
	                 ImGuiWindowFlags_AlwaysAutoResize)) {
		const auto playerPed = FindPlayerPed();
		if (ImGui::Button(local.get("tools.tp_to_marker").c_str())) {
			float x, y, z;
			Command<0x0AB6>(&x, &y, &z);
			z = CWorld::FindGroundZForCoord(x, y);
			playerPed->SetPosn(x, y, z);
			Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, 0);
			Command<Commands::SET_AREA_VISIBLE>(0);
			Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
		}
		if (ImGui::Button(local.get("tools.jetpack").c_str())) {
			Command<Commands::TASK_JETPACK>(static_cast<CPed*>(playerPed));
		}
		if (ImGui::Checkbox(local.get("tools.enexMarker").c_str(), &enexMarker)) {
			Command<Commands::DISABLE_ALL_ENTRY_EXITS>(static_cast<int>(enexMarker));
		}
		const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
		const int indexActor = utils::indexByUuid(actors, this->actorUuid_);
		utils::Combo(local.get("tools.tp_to_actor").c_str(), &this->actorUuid_, indexActor, actors.size(),
		             [&actors](const int i) {
			             return std::ref(actors.at(i)->getName());
		             }, [&actors](const int i) {
			             return actors.at(i)->getUuid();
		             });
		ImGui::SameLine();
		ImGui::PushID("teleport_to_actor");
		if (ImGui::Button(local.get("general.teleport").c_str())) {
			if (indexActor != -1) {
				//Command<Commands::SET_AREA_VISIBLE>(actors.at(indexActor));
				playerPed->SetPosn(actors.at(indexActor)->getPosition()[0], actors.at(indexActor)->getPosition()[1],
				                   actors.at(indexActor)->getPosition()[2]);
			}
		}
		ImGui::PopID();
		const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
		const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);
		utils::Combo(local.get("tools.tp_to_vehicle").c_str(), &this->vehicleUuid_, indexVehicle, vehicles.size(),
		             [&vehicles](const int i) {
			             return std::ref(vehicles.at(i)->getName());
		             }, [&vehicles](const int i) {
			             return vehicles.at(i)->getUuid();
		             });
		ImGui::SameLine();
		ImGui::PushID("teleport_to_vehicle");
		if (ImGui::Button(local.get("general.teleport").c_str())) {
			if (indexVehicle != -1) {
				//Command<Commands::SET_AREA_VISIBLE>(vehicles.at(indexVehicle));
				playerPed->SetPosn(vehicles.at(indexVehicle)->getPosition()[0],
				                   vehicles.at(indexVehicle)->getPosition()[1],
				                   vehicles.at(indexVehicle)->getPosition()[2]);
			}
		}
		ImGui::PopID();
		const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
		const int indexObject = utils::indexByUuid(objects, this->objectUuid_);
		utils::Combo(local.get("tools.tp_to_object").c_str(), &this->objectUuid_, indexObject, objects.size(),
		             [&objects](const int i) {
			             return std::ref(objects.at(i)->getName());
		             }, [&objects](const int i) {
			             return objects.at(i)->getUuid();
		             });
		ImGui::SameLine();
		ImGui::PushID("teleport_to_object");
		if (ImGui::Button(local.get("general.teleport").c_str())) {
			if (indexObject != -1) {
				//Command<Commands::SET_AREA_VISIBLE>(objects.at(indexObject));
				playerPed->SetPosn(objects.at(indexObject)->getPosition()[0],
				                   objects.at(indexObject)->getPosition()[1],
				                   objects.at(indexObject)->getPosition()[2]);
			}
		}
		ImGui::PopID();
		ImGui::Separator();

		if (ImGui::Button(local.get("antifreez").c_str())) {
			Command<Commands::SET_PLAYER_CONTROL>(0, true);
			playerPed->SetPosn(playerPed->GetPosition() + CVector(1, 1, 1));
		}
	}
	ImGui::End();
}
