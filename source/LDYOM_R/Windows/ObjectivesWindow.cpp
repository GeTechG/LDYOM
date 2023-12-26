#include "ObjectivesWindow.h"

#include <CCamera.h>
#include <CWorld.h>

#include "fa.h"
#include "fast_dynamic_cast.h"
#include "imgui.h"
#include "MathUtils.h"
#include "utils.h"
#include "../Data/AddMoneyPlayerObjective.h"
#include "../Data/AddTimerObjective.h"
#include "../Data/AddWeaponsPlayerObjective.h"
#include "../Data/AnimationActorObjective.h"
#include "../Data/AnimationPlayerObjective.h"
#include "../Data/CheckpointObjective.h"
#include "../Data/ClockTimeObjective.h"
#include "../Data/CollectPickupObjective.h"
#include "../Data/CountdownObjective.h"
#include "../Data/CutsceneObjective.h"
#include "../Data/DamageActorObjective.h"
#include "../Data/DamageObjectObjective.h"
#include "../Data/DestroyVehicleObjective.h"
#include "../Data/EndMissionObjective.h"
#include "../Data/EnterVehicleActorObjective.h"
#include "../Data/FollowPathActorObjective.h"
#include "../Data/FollowPathVehicleObjective.h"
#include "../Data/GetInVehicleObjective.h"
#include "../Data/GoToSceneObjective.h"
#include "../Data/JumpToObjectiveObjective.h"
#include "../Data/KillActorOrGroupObjective.h"
#include "../Data/LevelWantedPlayerObjective.h"
#include "../Data/PhoneCallPlayerObjective.h"
#include "../Data/PhotographObjectObjective.h"
#include "../Data/RemoveTimerObjective.h"
#include "../Data/RemoveWeaponsObjective.h"
#include "../Data/SaveObjective.h"
#include "../Data/StartMissionObjective.h"
#include "../Data/TeleportPlayerObjective.h"
#include "../Data/TeleportToVehiclePlayerObjective.h"
#include "../Data/TimeoutObjective.h"
#include "../Data/TouchObjectObjective.h"
#include "../Data/TrafficObjective.h"
#include "../Data/WaitSignalObjective.h"
#include "../Data/WeatherObjective.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

std::string Windows::ObjectivesWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_TASKS, Localization::getInstance().get("objective.objectives"));
}

std::string Windows::ObjectivesWindow::getNameOption() {
	return Localization::getInstance().get("objective.title");
}

int Windows::ObjectivesWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().
	                                                       size());
}

void Windows::ObjectivesWindow::createNewElement() {}

void Windows::ObjectivesWindow::createNewElementFrom(int i) {
	const auto &objective = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(i);

	switch (objective->getCategory()) {
		case 0:
			switch (objective->getTypeCategory()) {
				//World
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<CheckpointObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<CutsceneObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<CountdownObjective&>(*objective));
					break;
				case 3:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<TimeoutObjective&>(*objective));
					break;
				case 4:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<WeatherObjective&>(*objective));
					break;
				case 5:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<ClockTimeObjective&>(*objective));
					break;
				case 6:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<TrafficObjective&>(*objective));
					break;
				case 7:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<AddTimerObjective&>(*objective));
					break;
				case 8:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<RemoveTimerObjective&>(*objective));
					break;

				default:
					break;
			}
			break;

		case 1:
			switch (objective->getTypeCategory()) {
				//Actor
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<KillActorOrGroupObjective&>(*objective));
					break;

				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<DamageActorObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<FollowPathActorObjective&>(*objective));
					break;
				case 3:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<AnimationActorObjective&>(*objective));
					break;
				case 4:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<EnterVehicleActorObjective&>(*objective));
					break;

				default:
					break;
			}
			break;

		case 2:
			switch (objective->getTypeCategory()) {
				//Vehicle
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<GetInVehicleObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<DestroyVehicleObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<FollowPathVehicleObjective&>(*objective));
					break;

				default:
					break;
			}
			break;
		case 3:
			switch (objective->getTypeCategory()) {
				//Object
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<DamageObjectObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<PhotographObjectObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<TouchObjectObjective&>(*objective));
					break;

				default:
					break;
			}
			break;
		case 4:
			switch (objective->getTypeCategory()) {
				//Pickup
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<CollectPickupObjective&>(*objective));
					break;

				default:
					break;
			}
			break;
		case 5:
			switch (objective->getTypeCategory()) {
				//Player
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<TeleportPlayerObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<AnimationPlayerObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<TeleportToVehiclePlayerObjective&>(*objective));
					break;
				case 3:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<LevelWantedPlayerObjective&>(*objective));
					break;
				case 4:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<RemoveWeaponsObjective&>(*objective));
					break;
				case 5:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<AddWeaponsPlayerObjective&>(*objective));
					break;
				case 6:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<PhoneCallPlayerObjective&>(*objective));
					break;
				case 7:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<AddMoneyPlayerObjective&>(*objective));
					break;

				default:
					break;
			}
			break;
		case 6:
			switch (objective->getTypeCategory()) {
				//Mission
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<StartMissionObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<EndMissionObjective&>(*objective));
					break;
				default:
					break;
			}
			break;
		case 7:
			switch (objective->getTypeCategory()) {
				//Scene
				case 0:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<GoToSceneObjective&>(*objective));
					break;
				case 1:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<WaitSignalObjective&>(*objective));
					break;
				case 2:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<JumpToObjectiveObjective&>(*objective));
					break;
				case 3:
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectiveFrom(
						fast_dynamic_cast<SaveObjective&>(*objective));
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

std::string& Windows::ObjectivesWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(i)->getName();
}

void Windows::ObjectivesWindow::deleteElement(int i) {
	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().erase(begin + i);
	this->currentElement--;
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               open();
}

void Windows::ObjectivesWindow::drawOptions() {
	auto &local = Localization::getInstance();

	BaseObjective *objective = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(
		this->currentElement).get();

	listOverlays.emplace_back(local.get("info_overlay.objective_window"));

	objective->draw(local, this->listOverlays);
}

void Windows::ObjectivesWindow::onButtonCreateNewElement() {
	ImGui::OpenPopup("##modalObjectiveSelector");
}

void Windows::ObjectivesWindow::drawListWindow() {
	if (ImGui::BeginPopupModal("##modalObjectiveSelector", nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {
		if (ImGui::BeginMenu(Localization::getInstance().get("objective_categories.world").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.checkpoint").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					CheckpointObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.cutscene").c_str())) {
				auto &position = FindPlayerPed()->GetPosition();
				auto view = TheCamera.m_mCameraMatrix;
				view.Reorthogonalise();
				CQuaternion quaternion = MathUtils::matrixToQuat(view);
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					CutsceneObjective>(position, quaternion);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.countdown").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					CountdownObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.timeout").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					TimeoutObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.weather").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					WeatherObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.clock_time").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					ClockTimeObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.traffic").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					TrafficObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.add_timer").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					AddTimerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.remove_timer").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					RemoveTimerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("entities.actor").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.kill_actor_or_group").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					KillActorOrGroupObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.damage_actor").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					DamageActorObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.follow_path_actor").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					FollowPathActorObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.animation_actor").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					AnimationActorObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.enter_vehicle_actor").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					EnterVehicleActorObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("entities.vehicle").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.get_in_vehicle").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					GetInVehicleObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.destroy_vehicle").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					DestroyVehicleObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.follow_path_vehicle").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					FollowPathVehicleObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("entities.object").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.damage_object").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					DamageObjectObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.photograph_object").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					PhotographObjectObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.touch_object").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					TouchObjectObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("entities.pickup").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.collect_pickup").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					CollectPickupObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("general.player").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.teleport_player").c_str())) {
				auto &position = FindPlayerPed()->GetPosition();
				float heading = DEG(FindPlayerPed()->GetHeading());
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					TeleportPlayerObjective>(position, heading);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.animation_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					AnimationPlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.teleport_to_vehicle_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					TeleportToVehiclePlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.level_wanted_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					LevelWantedPlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.remove_weapons_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					RemoveWeaponsObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.add_weapons_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					AddWeaponsPlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.phone_call_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					PhoneCallPlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.add_money_player").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					AddMoneyPlayerObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("mission.title").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.start_mission").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					StartMissionObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.end_mission").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					EndMissionObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(Localization::getInstance().get("scenes.scene").c_str())) {
			if (ImGui::MenuItem(Localization::getInstance().get("objective.go_to_scene").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					GoToSceneObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.wait_signal").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					WaitSignalObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			if (ImGui::MenuItem(Localization::getInstance().get("objective.jump_to_objective").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					JumpToObjectiveObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}

			/*if (ImGui::MenuItem(Localization::getInstance().get("objective.save").c_str())) {
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<
					SaveObjective>(nullptr);
				this->selectElement(this->getListSize() - 1);
			}*/

			ImGui::EndMenu();
		}


		ImGui::EndPopup();
	}
}

void Windows::ObjectivesWindow::swapItems(const int firstItem, const int secondItem) {
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               close();
	const auto it = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().begin();
	std::iter_swap(it + firstItem, it + secondItem);
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               open();
}

void Windows::ObjectivesWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               close();
}

void Windows::ObjectivesWindow::open() {
	ListWindow::open();
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               open();
}

Windows::ObjectivesWindow::ObjectivesWindow(): ListWindow() {
	this->dragsItems = true;
}

void Windows::ObjectivesWindow::selectElementFirstCall(int i) {
	ListWindow::selectElement(i);
}

void Windows::ObjectivesWindow::selectElement(int i) {
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               close();

	ListWindow::selectElement(i);
	if (this->currentElement != -1)
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement)->
		                               open();
}
