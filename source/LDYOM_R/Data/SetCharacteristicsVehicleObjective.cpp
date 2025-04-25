#include "SetCharacteristicsVehicleObjective.h"

#include <extensions/ScriptCommands.h>

#include "ProjectsService.h"
#include "Settings.h"
#include "utilsRender.h"

SetCharacteristicsVehicleObjective::SetCharacteristicsVehicleObjective(void *_new) : BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.set_characteristics_vehicle"));
	this->name += suffix;
}

bool& SetCharacteristicsVehicleObjective::isBulletproof() { return bulletproof_; }
bool& SetCharacteristicsVehicleObjective::isFireproof() { return fireproof_; }
bool& SetCharacteristicsVehicleObjective::isExplosionproof() { return explosionproof_; }
bool& SetCharacteristicsVehicleObjective::isCollisionproof() { return collisionproof_; }
bool& SetCharacteristicsVehicleObjective::isMeleeproof() { return meleeproof_; }
bool& SetCharacteristicsVehicleObjective::isTiresVulnerability() { return tiresVulnerability_; }
bool& SetCharacteristicsVehicleObjective::isLocked() { return locked_; }
float& SetCharacteristicsVehicleObjective::getExtraPartsAngle() { return extraPartsAngle_; }
bool& SetCharacteristicsVehicleObjective::isIsLightsOn() { return isLightsOn_; }
std::array<float, 6>& SetCharacteristicsVehicleObjective::getOpenDoorsRation() { return openDoorsRation_; }
float& SetCharacteristicsVehicleObjective::getDirtyLevel() { return dirtyLevel_; }

void applyCharacteristics(SetCharacteristicsVehicleObjective *objective, CVehicle *vehicle) {
	using namespace plugin;

	vehicle->m_nPhysicalFlags.bBulletProof = static_cast<unsigned>(objective->isBulletproof());
	vehicle->m_nPhysicalFlags.bCollisionProof = static_cast<unsigned>(objective->isCollisionproof());
	vehicle->m_nPhysicalFlags.bExplosionProof = static_cast<unsigned>(objective->isExplosionproof());
	vehicle->m_nPhysicalFlags.bFireProof = static_cast<unsigned>(objective->isFireproof());
	vehicle->m_nPhysicalFlags.bMeleeProof = static_cast<unsigned>(objective->isMeleeproof());
	Command<Commands::SET_CAN_BURST_CAR_TYRES>(vehicle, !objective->isTiresVulnerability());

	static_cast<CAutomobile*>(vehicle)->UpdateMovingCollision(
		objective->getExtraPartsAngle());
	Command<Commands::FORCE_CAR_LIGHTS>(vehicle,
	                                    objective->isIsLightsOn());
	vehicle->m_fDirtLevel = objective->getDirtyLevel();
	for (int i = 0; i < 6; i++) {
		const int carNodeIndexFromDoor = utils::GetCarNodeIndexFromDoor(i);
		vehicle->OpenDoor(nullptr, carNodeIndexFromDoor, static_cast<eDoors>(i),
		                  objective->getOpenDoorsRation()[i], false);
	}
	vehicle->m_eDoorLock = objective->isLocked() ? DOORLOCK_LOCKED : DOORLOCK_UNLOCKED;
}

void openDoors(Localization &local, SetCharacteristicsVehicleObjective *this_, Vehicle *vehicle) {
	if (ImGui::TreeNode(local.get("vehicle.open_doors").c_str())) {
		const auto scaleFont = ImGui::GetFontSize() / 16.f;
		ImGui::BeginChild("##openDoors", ImVec2(0, scaleFont * 200.f));
		ImGui::PushItemWidth(scaleFont * 150.0f);
		for (int i = 0; i < 6; i++) {
			if (ImGui::SliderFloat(local.getArray("vehicle.components")[i + 4].c_str(),
			                       &this_->getOpenDoorsRation()[i],
			                       0.f, 1.f)) {
				applyCharacteristics(this_, vehicle->getEditorVehicle().value());
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();
		ImGui::TreePop();
	}
}

void SetCharacteristicsVehicleObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	using namespace plugin;

	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	IncorrectHighlight(indexVehicle == -1, [&] {
		utils::Combo(local.get("entities.vehicle").c_str(), &this->vehicleUuid_, indexVehicle, vehicles.size(),
		             [&vehicles](const int i) {
			             return std::ref(vehicles.at(i)->getName());
		             }, [&vehicles](const int i) {
			             return vehicles.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (indexVehicle == -1) {
			if (!vehicles.empty()) {
				this->vehicleUuid_ = vehicles.back()->getUuid();
			}
		}
	}

	if (indexVehicle != -1) {
		const auto vehicle = vehicles.at(indexVehicle).get();

		if (ImGui::SliderAngle(local.get("vehicle.extra_parts_angle").c_str(), &this->getExtraPartsAngle(), 0.f,
		                       360.f)) {
			applyCharacteristics(this, vehicle->getEditorVehicle().value());
		}
		ImGui::SetItemTooltip(local.get("vehicle.extra_parts_angle_info").c_str());

		if (ImGui::Checkbox(local.get("vehicle.lights_on").c_str(), &this->isIsLightsOn())) {
			applyCharacteristics(this, vehicle->getEditorVehicle().value());
		}

		if (ImGui::SliderFloat(local.get("vehicle.dirty_level").c_str(), &this->getDirtyLevel(), 0.f, 15.f)) {
			applyCharacteristics(this, vehicle->getEditorVehicle().value());
		}

		ImGui::Separator();

		const auto &vehicleUnbreakable = local.getArray("vehicle.vehicle_unbreakable");
		utils::ToggleButton(vehicleUnbreakable[0].c_str(), &this->isBulletproof());
		utils::ToggleButton(vehicleUnbreakable[1].c_str(), &this->isFireproof());
		utils::ToggleButton(vehicleUnbreakable[2].c_str(), &this->isExplosionproof());
		utils::ToggleButton(vehicleUnbreakable[3].c_str(), &this->isCollisionproof());
		utils::ToggleButton(vehicleUnbreakable[4].c_str(), &this->isMeleeproof());
		utils::ToggleButton(vehicleUnbreakable[5].c_str(), &this->isTiresVulnerability());

		ImGui::Separator();

		openDoors(local, this, vehicle);

		utils::ToggleButton(local.get("vehicle.locked").c_str(), &this->isLocked());


		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		auto position = vehicles.at(indexVehicle)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

ktwait SetCharacteristicsVehicleObjective::execute(Scene *scene, Vehicle *vehicle, Result &result,
                                                   ktcoro_tasklist &tasklist) {
	applyCharacteristics(this, vehicle->getProjectVehicle().value());

	co_return;
}

void SetCharacteristicsVehicleObjective::open() {
	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);
	if (indexVehicle != -1) {
		auto &vehicle = vehicles.at(indexVehicle);
		applyCharacteristics(this, vehicle->getEditorVehicle().value());
	}
}

void SetCharacteristicsVehicleObjective::close() {
	VehicleObjective::close();
	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);
	if (indexVehicle != -1) {
		auto &vehicle = vehicles.at(indexVehicle);
		vehicle->spawnEditorVehicle();
	}
}
