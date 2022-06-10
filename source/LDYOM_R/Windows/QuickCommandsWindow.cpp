#include "QuickCommandsWindow.h"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "EditByPlayerService.h"
#include "FastObjectSelector.h"
#include "imgui_internal.h"
#include "ProjectsService.h"
#include "utils.h"
#include "WindowsRenderService.h"
#include "Localization/Localization.h"

extern bool openWindowsMenu;

void duplicate(Localization& local) {
	if (ImGui::BeginMenu(local.get("quick_commands.duplicate").c_str())) {

		auto playerPosition = [](IPositionable& positionable) {
			const CVector playerPosition = FindPlayerPed()->GetPosition();
			positionable.getPosition()[0] = playerPosition.x;
			positionable.getPosition()[1] = playerPosition.y;
			positionable.getPosition()[2] = playerPosition.z;
		};

		auto playerAngle = [](float& angle) {
			const float playerAngleRotation = FindPlayerPed()->GetHeading();
			angle = DEG(playerAngleRotation);
		};

		if (ImGui::BeginMenu(local.get("entities.actors").c_str())) {
			for (const auto &actor : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors()) {
				ImGui::PushID(reinterpret_cast<int>(actor.get()));
				if (ImGui::MenuItem(actor->getName())) {
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewActorFrom(*actor);
					auto &newActor = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().back();
					playerPosition(*newActor);
					playerAngle(newActor->getHeadingAngle());
					newActor->spawnEditorPed();
				}
				ImGui::PopID();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(local.get("entities.vehicles").c_str())) {
			for (const auto& vehicle : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles()) {
				ImGui::PushID(reinterpret_cast<int>(vehicle.get()));
				if (ImGui::MenuItem(vehicle->getName())) {
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVehicleFrom(*vehicle);
					const auto &newVehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().back();
					playerPosition(*newVehicle);
					playerAngle(newVehicle->getHeadingAngle());
					newVehicle->spawnEditorVehicle(false);
				}
				ImGui::PopID();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void Windows::QuickCommandsWindow::draw() {
	auto &local = Localization::getInstance();

	const auto center = ImGui::GetIO().DisplaySize / ImVec2(2.0f, 2.0f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin(local.get("quick_commands.title").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		duplicate(local);

		if (ImGui::MenuItem(local.get("quick_commands.quick_create_object").c_str())) {
			openWindowsMenu = true;

			const auto fastObjectSelector = WindowsRenderService::getInstance().getWindow<FastObjectSelector>();
			fastObjectSelector->openWithCallback([](int modelId) {
				const auto currentScene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();
				currentScene->createNewObject();
				const auto& object = currentScene->getObjects().back();
				object->getModelId() = modelId;
				object->spawnEditorObject();
				EditByPlayerService::getInstance().positionalObject(object->getEditorObject().value(), object->getPosition(), object->getRotations());
				});
		}
	}

	ImGui::End();
}
