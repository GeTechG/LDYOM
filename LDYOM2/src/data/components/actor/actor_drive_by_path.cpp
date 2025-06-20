#include "actor_drive_by_path.h"
#include "actor_behaviour.h"
#include "components/actor.h"
#include "components/vehicle.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <CWorld.h>
#include <entities_manager.h>
#include <in_game/actor_paths_editing.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <window_manager.h>

void components::ActorDriveByPath::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorDriveByPath>("ActorDriveByPathComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorDriveByPath, cast, nearestVehicle, vehicleId,
	                 teleportToVehicle, speedType, points, leaveVehicle, isLooped);
}

components::ActorDriveByPath::ActorDriveByPath()
	: Component(TYPE) {}

void components::ActorDriveByPath::run() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (!actor || !actor->ped) {
		return;
	}

	auto vehicle = findTargetVehicle(actor);
	if (!vehicle || !vehicle->handle) {
		return;
	}

	ProjectPlayer::instance().projectTasklist->add_task(enterVehicle, this, actor, vehicle);
}

std::shared_ptr<components::Vehicle>
components::ActorDriveByPath::findTargetVehicle(std::shared_ptr<components::Actor> actor) {
	return nearestVehicle ? findNearestVehicle(actor) : findVehicleById();
}

std::shared_ptr<components::Vehicle>
components::ActorDriveByPath::findNearestVehicle(std::shared_ptr<components::Actor> actor) {
	auto entities = ProjectPlayer::instance().getEntities();
	std::vector<Entity*> vehicles;
	std::ranges::copy_if(entities, std::back_inserter(vehicles),
	                     [](Entity* entity) { return entity->hasComponent(Vehicle::TYPE); });

	if (vehicles.empty()) {
		return nullptr;
	}

	if (vehicles.size() == 1) {
		return Vehicle::cast(vehicles.front()->getComponent(Vehicle::TYPE));
	}

	auto nearestVehicleIt = std::ranges::min_element(vehicles, [actor](Entity* a, Entity* b) {
		const auto aVehicle = Vehicle::cast(a->getComponent(Vehicle::TYPE));
		const auto bVehicle = Vehicle::cast(b->getComponent(Vehicle::TYPE));
		if (!aVehicle || !bVehicle) {
			return false;
		}
		return DistanceBetweenPoints(aVehicle->handle->GetPosition(), actor->ped->GetPosition()) <
		       DistanceBetweenPoints(bVehicle->handle->GetPosition(), actor->ped->GetPosition());
	});

	return nearestVehicleIt != vehicles.end() ? Vehicle::cast((*nearestVehicleIt)->getComponent(Vehicle::TYPE))
	                                          : nullptr;
}

std::shared_ptr<components::Vehicle> components::ActorDriveByPath::findVehicleById() {
	auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleIt = std::ranges::find_if(entities, [vehicleUuid](Entity* entity) {
		return entity->id == vehicleUuid && entity->hasComponent(Vehicle::TYPE);
	});

	return vehicleIt != entities.end() ? Vehicle::cast((*vehicleIt)->getComponent(Vehicle::TYPE)) : nullptr;
}

ktwait components::ActorDriveByPath::enterVehicle(ActorDriveByPath* _this, std::shared_ptr<components::Actor> actor,
                                                  std::shared_ptr<components::Vehicle> vehicle) {
	int marker;
	plugin::Command<plugin::Commands::LOAD_CHAR_DECISION_MAKER>(0, &marker);
	plugin::Command<plugin::Commands::SET_CHAR_DECISION_MAKER>(actor->getPedRef(), marker);
	if (_this->teleportToVehicle) {
		plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(actor->getPedRef(),
		                                                                     vehicle->getVehicleRef());
	} else {
		plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_DRIVER>(actor->getPedRef(), vehicle->getVehicleRef(), 2900);
		co_await 3000;
	}
	auto speed = 0.0f;
	auto driveType = 0;
	auto radius = 0.0f;
	switch (_this->speedType) {
		case 0: // Normal
			{
				driveType = 0; // Normal driving
				speed = 20.0f; // Normal speed
				radius = 5.0f; // Normal radius
				break;
			}
		case 1: // Fast
			{
				driveType = 2; // Fast driving
				speed = 30.0f; // Fast speed
				radius = 7.0f; // Fast radius
				break;
			}
		case 2: // Very fast
			{
				driveType = 2;  // Very fast driving
				speed = 40.0f;  // Very fast speed
				radius = 10.0f; // Very fast radius
				break;
			}
		default: break;
	}
	auto vehiclePos = vehicle->handle->GetPosition();
	float groundZ;
	plugin::Command<plugin::Commands::GET_GROUND_Z_FOR_3D_COORD>(vehiclePos.x, vehiclePos.y, vehiclePos.z, &groundZ);
	vehiclePos.z -= groundZ;
	size_t indexPoint = 0;
	while (IS_PLAYING) {
		if (indexPoint >= _this->points.size()) {
			if (_this->isLooped) {
				indexPoint = 0;
			} else {
				break;
			}
		}
		const auto& point = _this->points[indexPoint];
		float nodeX, nodeY, nodeZ;
		plugin::Command<plugin::Commands::GET_CLOSEST_CAR_NODE>(point[0], point[1], point[2], &nodeX, &nodeY, &nodeZ);
		const auto nodeDistance = DistanceBetweenPoints(CVector2D(nodeX, nodeY), CVector2D(vehiclePos.x, vehiclePos.y));
		if (nodeDistance > 10.0f) {
			plugin::Command<plugin::Commands::TASK_CAR_DRIVE_TO_COORD>(
				actor->getPedRef(), vehicle->getVehicleRef(), point[0], point[1], point[2], speed, 2, 0, driveType);
			plugin::Command<plugin::Commands::SET_CAR_STRONG>(vehicle->getVehicleRef(),
			                                                  true); // Make the vehicle strong
		} else {
			plugin::Command<plugin::Commands::TASK_CAR_DRIVE_TO_COORD>(actor->getPedRef(), vehicle->getVehicleRef(),
			                                                           point[0], point[1], point[2], speed, 3, 0, 8);
		}
		co_await 10;
		if (!plugin::Command<plugin::Commands::DOES_CHAR_EXIST>(actor->getPedRef()) ||
		    !plugin::Command<plugin::Commands::DOES_VEHICLE_EXIST>(vehicle->getVehicleRef()) ||
		    plugin::Command<plugin::Commands::IS_CHAR_DEAD>(actor->getPedRef()) ||
		    plugin::Command<plugin::Commands::IS_CAR_DEAD>(vehicle->getVehicleRef())) {
			co_return;
		}
		while (!plugin::Command<plugin::Commands::LOCATE_CAR_2D>(vehicle->getVehicleRef(), point[0], point[1], radius,
		                                                         radius, false)) {
			co_await 10;
		}
		indexPoint++;
	}
	plugin::Command<plugin::Commands::TASK_CAR_TEMP_ACTION>(actor->getPedRef(), vehicle->getVehicleRef(), 6,
	                                                        1000); // Handbrake straight
	co_await 1000;
	if (_this->leaveVehicle) {
		plugin::Command<plugin::Commands::TASK_LEAVE_ANY_CAR>(actor->getPedRef());
	}
	auto entities = ProjectPlayer::instance().getEntities();
	std::vector<Entity*> actors;
	std::ranges::copy_if(entities, std::back_inserter(actors),
	                     [](Entity* entity) { return entity->hasComponent(Actor::TYPE); });
	for (auto& actorEntity : actors) {
		auto actor = Actor::cast(actorEntity->getComponent(Actor::TYPE));
		if (actor && actor->ped && vehicle->handle) {
			if (plugin::Command<plugin::Commands::IS_CHAR_IN_CAR>(actor->getPedRef(), vehicle->getVehicleRef())) {
				auto actorBehavior = ActorBehaviour::cast(actorEntity->getComponent(ActorBehaviour::TYPE));
				if (actorBehavior && actorBehavior->directAttack) {
					plugin::Command<plugin::Commands::TASK_KILL_CHAR_ON_FOOT>(actor->getPedRef(),
					                                                          CPools::GetPedRef(FindPlayerPed()));
				}
			}
		}
	}
}

nlohmann::json components::ActorDriveByPath::to_json() const {
	auto j = Component::to_json();
	j["nearestVehicle"] = nearestVehicle;
	j["vehicleId"] = vehicleId;
	j["teleportToVehicle"] = teleportToVehicle;
	j["speedType"] = speedType;
	j["points"] = points;
	j["isLooped"] = isLooped;
	j["leaveVehicle"] = leaveVehicle;
	return j;
}

void components::ActorDriveByPath::from_json(const nlohmann::json& j) {
	Component::from_json(j);
	nearestVehicle = j.value("nearestVehicle", false);
	vehicleId = j.value("vehicleId", std::string{});
	teleportToVehicle = j.value("teleportToVehicle", false);
	speedType = j.value("speedType", 0);
	points = j.value("points", std::vector<std::array<float, 3>>{});
	isLooped = j.value("isLooped", false);
	leaveVehicle = j.value("leaveVehicle", false);
}

void components::ActorDriveByPath::editorRender() {
	constexpr float labelWidth = 0.45f;

	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Speed type selection
	ImGui::Text(tr("speed_type").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##speedType", tr(fmt::format("speed_types.{}", speedType)).c_str())) {
		for (int i = 0; i < 3; ++i) {
			const bool isSelected = (i == speedType);
			if (ImGui::Selectable(tr(fmt::format("speed_types.{}", i)).c_str(), isSelected)) {
				speedType = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("loop").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##loop", &isLooped);

	if (ImGui::Button(tr("edit_path").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		ActorPathsEditing::addJetpack();
		ActorPathsEditing::openPathEditor(points, [this](bool saveChanges, const PointsArray& points) {
			if (saveChanges) {
				this->points = points;
			}
			WindowManager::instance().disableWindowRendering(false);
		});
	}

	// Nearest vehicle checkbox
	ImGui::Text(tr("nearest_vehicle").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##nearestVehicle", &nearestVehicle);

	if (!nearestVehicle) {
		renderVehicleSelection(availableWidth, labelWidth);
	}

	// Teleport to vehicle checkbox
	ImGui::Text(tr("teleport_to_vehicle").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##teleportToVehicle", &teleportToVehicle);

	// Leave vehicle checkbox
	ImGui::Text(tr("leave_vehicle").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##leaveVehicle", &leaveVehicle);
}

void components::ActorDriveByPath::renderVehicleSelection(float availableWidth, float labelWidth) {
	ImGui::Text(tr("vehicle_id").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);

	const auto vehicles = EntitiesManager::instance().getEntitiesWithComponent(Vehicle::TYPE);
	const auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	const auto currentVehicle =
		std::ranges::find_if(vehicles, [vehicleUuid](Entity* entity) { return entity->id == vehicleUuid; });

	const char* previewText = (currentVehicle != vehicles.end()) ? (*currentVehicle)->name.c_str() : "";

	if (ImGui::BeginCombo("##vehicleId", previewText)) {
		for (size_t i = 0; i < vehicles.size(); ++i) {
			const auto& vehicle = vehicles[i];
			const bool isSelected = (vehicle->id == vehicleUuid);

			if (ImGui::Selectable(fmt::format("{}##{}", vehicle->name, i).c_str(), isSelected)) {
				vehicleId = to_string(vehicle->id);
			}
		}
		ImGui::EndCombo();
	}
}

void components::ActorDriveByPath::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() {
				ProjectPlayer::instance().projectTasklist->add_task(
					[this](ActorDriveByPath* _this) -> ktwait {
						_this->run();
						co_return;
					},
					this);
			}));
		}
	}
}

void components::ActorDriveByPath::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorDriveByPath::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorDriveByPath::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }