#include "QuickCommandsWindow.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include <CWorld.h>

#include "EditByPlayerService.h"
#include "FastObjectSelector.h"
#include "ProjectsService.h"
#include "utils.h"
#include "WindowsRenderService.h"
#include "Localization/Localization.h"

extern bool openWindowsMenu;

void duplicate(Localization &local) {
	if (ImGui::BeginMenu(local.get("quick_commands.duplicate").c_str())) {
		auto playerPosition = [](IPositionable &positionable) {
			const CVector playerPosition = FindPlayerPed()->GetPosition();
			positionable.getPosition()[0] = playerPosition.x;
			positionable.getPosition()[1] = playerPosition.y;
			positionable.getPosition()[2] = playerPosition.z;
		};

		auto playerAngle = [](float &angle) {
			const float playerAngleRotation = FindPlayerPed()->GetHeading();
			angle = DEG(playerAngleRotation);
		};

		auto duplicateEntities = [&]<typename T>(const char *nameCategory, std::vector<std::unique_ptr<T>> &entities,
		                                         auto newEntityCall, auto setCustoms) {
			if (ImGui::BeginMenu(local.get(nameCategory).c_str())) {
				for (const auto &entity : entities) {
					ImGui::PushID(reinterpret_cast<int>(entity.get()));
					if (ImGui::MenuItem(entity->getName())) {
						newEntityCall(ProjectsService::getInstance().getCurrentProject().getCurrentScene(), entity);
						auto &newEntity = entities.back();
						playerPosition(*newEntity);
						setCustoms(newEntity);
						ImGui::PopID();
						break;
					}
					ImGui::PopID();
				}
				ImGui::EndMenu();
			}
		};

		duplicateEntities(
			"entities.actors",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors(),
			[](Scene *scene, const std::unique_ptr<Actor> &entity) {
				scene->createNewActorFrom(*entity);
			},
			[&](const std::unique_ptr<Actor> &actor) {
				playerAngle(actor->getHeadingAngle());
				actor->spawnEditorPed();
			});

		duplicateEntities(
			"entities.vehicles",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles(),
			[](Scene *scene, const std::unique_ptr<Vehicle> &entity) { scene->createNewVehicleFrom(*entity); },
			[&](const std::unique_ptr<Vehicle> &entity) {
				playerAngle(entity->getHeadingAngle());
				entity->spawnEditorVehicle();
			});

		duplicateEntities(
			"entities.objects",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects(),
			[](Scene *scene, const std::unique_ptr<Object> &entity) {
				scene->createNewObjectFrom(*entity);
			},
			[&](const std::unique_ptr<Object> &entity) {
				entity->spawnEditorObject();
			});

		duplicateEntities(
			"entities.particles",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles(),
			[](Scene *scene, const std::unique_ptr<Particle> &entity) {
				scene->createNewParticleFrom(*entity);
			},
			[&](const std::unique_ptr<Particle> &entity) {
				entity->spawnEditorParticle();
			});

		duplicateEntities(
			"entities.trains",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains(),
			[](Scene *scene, const std::unique_ptr<Train> &entity) {
				scene->createNewTrainFrom(*entity);
			},
			[&](const std::unique_ptr<Train> &entity) {
				entity->spawnEditorTrain();
			});

		duplicateEntities(
			"entities.pickups",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups(),
			[](Scene *scene, const std::unique_ptr<Pickup> &entity) {
				scene->createNewPickupFrom(*entity);
			},
			[&](const std::unique_ptr<Pickup> &entity) {
				entity->spawnEditorPickup();
			});

		duplicateEntities(
			"entities.pyrotechnics",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics(),
			[](Scene *scene, const std::unique_ptr<Pyrotechnics> &entity) {
				scene->createNewPyrotechnicsFrom(*entity);
			},
			[&](const std::unique_ptr<Pyrotechnics> &entity) {
				entity->spawnEditorPyrotechnics();
			});

		duplicateEntities(
			"entities.audio",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio(),
			[](Scene *scene, const std::unique_ptr<Audio> &entity) {
				scene->createNewAudioFrom(*entity);
			},
			[&](const std::unique_ptr<Audio> &entity) {
				entity->spawnEditorAudio();
			});

		duplicateEntities(
			"entities.visual_effects",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects(),
			[](Scene *scene, const std::unique_ptr<VisualEffect> &entity) {
				scene->createNewVisualEffectFrom(*entity);
			},
			[&](const std::unique_ptr<VisualEffect> &entity) {
				entity->spawnEditorVisualEffect();
			});

		duplicateEntities(
			"entities.checkpoints",
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints(),
			[](Scene *scene, const std::unique_ptr<Checkpoint> &entity) {
				scene->createNewCheckpointFrom(*entity);
			},
			[&](const std::unique_ptr<Checkpoint> &entity) {
				playerAngle(entity->getAngle());
				entity->spawnEditorCheckpoint();
			});

		ImGui::EndMenu();
	}
}

void loadScene(Localization &local) {
	if (ImGui::BeginMenu(local.get("scenes.load_scene").c_str())) {
		for (auto &pair : ProjectsService::getInstance().getCurrentProject().getScenes()) {
			if (ImGui::MenuItem(pair.second->getName())) {
				ProjectsService::getInstance().getCurrentProject().changeScene(pair.first);
			}
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
		loadScene(local);

		if (ImGui::MenuItem(local.get("quick_commands.quick_create_object").c_str())) {
			const auto fastObjectSelector = WindowsRenderService::getInstance().getWindow<FastObjectSelector>();
			openWindowsMenu = true;
			fastObjectSelector->openWithCallback([](int modelId) {
				const auto currentScene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();
				currentScene->createNewObject();
				const auto &object = currentScene->getObjects().back();
				object->getModelId() = modelId;
				object->spawnEditorObject();
				EditByPlayerService::getInstance().positionalObject(
					object.get()->getEditorObject().value(),
					[&object](CMatrix &mat) {
						const auto obj = object.get()->getEditorObject()
						                       .value();
						CWorld::Remove(obj);
						obj->SetMatrix(mat);
						obj->UpdateRwFrame();
						CWorld::Add(obj);
					}, object->getPosition(), object->getRotations()
				);
			});
		}
	}

	ImGui::End();
}
