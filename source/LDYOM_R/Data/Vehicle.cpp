#include "Vehicle.h"

#include <array>
#include <CPools.h>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "Scene.h"
#include "Tasker.h"
#include "utils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

void Vehicle::recolorVehicle(const bool recolor, CVehicle *const newVehicle) {
	if (recolor) {
		colors_.clear();
		components::extractComObjMatVehicle(
			newVehicle, [&](int i, components::VehicleComponent comp, components::VehicleAtomic obj,
			                components::VehicleMaterial mat) {
				const CRGBA rgba = mat.getColor();
				CRGBA color;
				unsigned char type;
				if (rgba.r == 0x3C && rgba.g == 0xFF && rgba.b == 0x00) {
					color.Set(utils::getCarColorRgba(newVehicle->m_nPrimaryColor));
					primaryColor_[0] = static_cast<float>(color.a) / 255.0f;
					primaryColor_[1] = static_cast<float>(color.b) / 255.0f;
					primaryColor_[2] = static_cast<float>(color.g) / 255.0f;
					primaryColor_[3] = static_cast<float>(color.r) / 255.0f;
					type = 0;
				} else if (rgba.r == 0xFF && rgba.g == 0x00 && rgba.b == 0xAF) {
					color.Set(utils::getCarColorRgba(newVehicle->m_nSecondaryColor));
					secondaryColor_[0] = static_cast<float>(color.a) / 255.0f;
					secondaryColor_[1] = static_cast<float>(color.b) / 255.0f;
					secondaryColor_[2] = static_cast<float>(color.g) / 255.0f;
					secondaryColor_[3] = static_cast<float>(color.r) / 255.0f;
					type = 1;
				} else {
					type = 2;
				}
				array color_rgba{
					static_cast<float>(rgba.r) / 255.0f,
					static_cast<float>(rgba.g) / 255.0f,
					static_cast<float>(rgba.b) / 255.0f,
					static_cast<float>(rgba.a) / 255.0f
				};
				colors_.emplace_back(type, color_rgba);
			});
	} else {
		components::extractComObjMatVehicle(
			newVehicle, [&](const int i, components::VehicleComponent comp, components::VehicleAtomic obj,
			                components::VehicleMaterial mat) {
				const float newR = colors_.at(i).second[0],
				            newG = colors_.at(i).second[1],
				            newB = colors_.at(i).second[2],
				            newA = colors_.at(i).second[3];
				const CRGBA old = mat.getColor();
				if (!(static_cast<unsigned char>(floorf(newR * 255.0f)) == old.r &&
					static_cast<unsigned char>(floorf(newG * 255.0f)) == old.g &&
					static_cast<unsigned char>(floorf(newB * 255.0f)) == old.b &&
					static_cast<unsigned char>(floorf(newA * 255.0f)) == old.a))
					mat.setColor(floatColorToCRGBA(colors_[i].second));
			});
	}
}

CVehicle* Vehicle::spawnVehicle(bool recolor) {
	const auto model = ModelsService::validVehiceModel(this->modelId_);
	CStreaming::RequestModel(model, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	Command<Commands::SET_CAR_MODEL_COMPONENTS>(model, componentTypeA_, componentTypeB_);
	if (!numberplate_.empty()) {
		Command<Commands::CUSTOM_PLATE_FOR_NEXT_CAR>(model, numberplate_.c_str());
	}
	Command<Commands::CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR>(model, numberplateCity_);

	int newVehicleHandle;
	Command<Commands::CREATE_CAR>(model, this->pos_[0], this->pos_[1], this->pos_[2], &newVehicleHandle);
	Command<Commands::SET_VEHICLE_AREA_VISIBLE>(newVehicleHandle, this->interiorId);
	const auto newVehicle = CPools::GetVehicle(newVehicleHandle);

	CStreaming::SetMissionDoesntRequireModel(model);

	newVehicle->m_nVehicleFlags.bEngineOn = 0;
	newVehicle->m_fHealth = static_cast<float>(this->health_);
	if (recolor) {
		this->paintjob_ = -1;
	}
	if (this->paintjob_ != -1) {
		newVehicle->SetRemap(this->paintjob_);
	}
	for (int i = 0; i < 17; ++i) {
		if (this->damagesComponents_[i] < 199.f) {
			static_cast<CAutomobile*>(newVehicle)->m_damageManager.ApplyDamage(
				static_cast<CAutomobile*>(newVehicle), static_cast<tComponent>(i + 1), damagesComponents_[i], 1.f);
		} else {
			if (i + 1 <= 4) {
				Command<Commands::BURST_CAR_TYRE>(newVehicle, i);
			} else if (i + 1 <= 10) {
				const auto door = i + 1 - 5;
				Command<Commands::POP_CAR_DOOR>(newVehicle, door, false);
			} else {
				const auto panel = i + 1 - 11;
				Command<Commands::POP_CAR_PANEL>(newVehicle, panel, false);
			}
		}
	}
	static_cast<CAutomobile*>(newVehicle)->UpdateMovingCollision(this->extraPartsAngle_);
	Command<Commands::FORCE_CAR_LIGHTS>(newVehicle,
	                                    this->isIsLightsOn() ? 2 : 0);
	for (int i = 0; i < 6; ++i) {
		const int carNodeIndexFromDoor = utils::GetCarNodeIndexFromDoor(i);
		newVehicle->OpenDoor(nullptr, carNodeIndexFromDoor, static_cast<eDoors>(i),
		                     this->getOpenDoorsRation()[i], false);
	}
	newVehicle->m_eDoorLock = this->isLocked() ? DOORLOCK_LOCKED : DOORLOCK_UNLOCKED;
	newVehicle->m_fDirtLevel = this->dirtyLevel_;
	Command<Commands::SET_CAR_HEAVY>(newVehicle, this->isHeavy());

	restoreUpgrades(newVehicle, recolor);

	newVehicle->m_nPhysicalFlags.bBulletProof = static_cast<unsigned>(this->isBulletproof());
	newVehicle->m_nPhysicalFlags.bCollisionProof = static_cast<unsigned>(this->isCollisionproof());
	newVehicle->m_nPhysicalFlags.bExplosionProof = static_cast<unsigned>(this->isExplosionproof());
	newVehicle->m_nPhysicalFlags.bFireProof = static_cast<unsigned>(this->isFireproof());
	newVehicle->m_nPhysicalFlags.bMeleeProof = static_cast<unsigned>(this->isMeleeproof());
	Command<Commands::SET_CAN_BURST_CAR_TYRES>(newVehicle, !this->isTiresVulnerability());

	return newVehicle;
}

Vehicle::Vehicle(const char *name, const CVector &pos, float headingAngle): ObjectiveDependent(nullptr),
                                                                            uuid_(boost::uuids::random_generator()()),
                                                                            headingAngle_(headingAngle),
                                                                            modelId_(400), health_(1000),
                                                                            numberplateCity_(-1) {
	this->name = name;
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
	this->upgrades_.fill(-1);
	this->damagesComponents_.fill(0.f);
	this->openDoorsRation_.fill(0.f);
	Command<Commands::GET_AREA_VISIBLE>(&this->interiorId);
}

Vehicle Vehicle::copy() const {
	Vehicle vehicle(*this);
	vehicle.uuid_ = boost::uuids::random_generator()();
	vehicle.name += " (copy)";
	Command<Commands::GET_AREA_VISIBLE>(&vehicle.interiorId);

	return vehicle;
}

Vehicle::~Vehicle() {
	this->deleteEditorVehicle();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Vehicle::getUuid() {
	return uuid_;
}

std::optional<CVehicle*>& Vehicle::getEditorVehicle() {
	return editorVehicle_;
}

std::optional<CVehicle*>& Vehicle::getProjectVehicle() {
	return projectVehicle_;
}

int& Vehicle::getModelId() {
	return modelId_;
}

bool& Vehicle::isShouldNotDie() {
	return shouldNotDie_;
}

int& Vehicle::getHealth() {
	return health_;
}

bool& Vehicle::isBulletproof() {
	return bulletproof_;
}

bool& Vehicle::isFireproof() {
	return fireproof_;
}

bool& Vehicle::isExplosionproof() {
	return explosionproof_;
}

bool& Vehicle::isCollisionproof() {
	return collisionproof_;
}

bool& Vehicle::isMeleeproof() {
	return meleeproof_;
}

bool& Vehicle::isTiresVulnerability() {
	return tiresVulnerability_;
}

bool& Vehicle::isExtendedColor() {
	return extendedColor_;
}

bool& Vehicle::isLocked() {
	return locked_;
}

std::vector<std::pair<unsigned char, std::array<float, 4>>>& Vehicle::getColors() {
	return colors_;
}

int& Vehicle::getComponentTypeA() {
	return componentTypeA_;
}

int& Vehicle::getComponentTypeB() {
	return componentTypeB_;
}

int& Vehicle::getNumberplateCity() {
	return numberplateCity_;
}

float* Vehicle::getPrimaryColor() {
	return this->primaryColor_;
}

float* Vehicle::getSecondaryColor() {
	return this->secondaryColor_;
}

std::string& Vehicle::getNumberplate() {
	return this->numberplate_;
}

std::array<int, 15>& Vehicle::getUpgrades() { return upgrades_; }
int& Vehicle::getPaintjob() { return paintjob_; }
std::array<float, 17>& Vehicle::getDamagesComponents() { return damagesComponents_; }
float& Vehicle::getExtraPartsAngle() { return extraPartsAngle_; }
bool& Vehicle::isIsLightsOn() { return isLightsOn_; }
std::array<float, 6>& Vehicle::getOpenDoorsRation() { return openDoorsRation_; }
float& Vehicle::getDirtyLevel() { return dirtyLevel_; }
bool& Vehicle::isHeavy() { return heavy_; }
int& Vehicle::getInteriorId() { return interiorId; }


void Vehicle::updateLocation() const {
	if (this->editorVehicle_.has_value()) {
		this->editorVehicle_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorVehicle_.value()->SetHeading(RAD(this->headingAngle_));
	}
	if (this->projectVehicle_.has_value()) {
		this->projectVehicle_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->projectVehicle_.value()->SetHeading(RAD(this->headingAngle_));
	}
}

std::string& Vehicle::getName() {
	return this->name;
}

float* Vehicle::getPosition() {
	return pos_;
}

float& Vehicle::getHeadingAngle() {
	return headingAngle_;
}

void Vehicle::setEditorPrimaryColor() {
	components::extractComObjMatVehicle(this->editorVehicle_.value(),
	                                    [&](int i, components::VehicleComponent comp, components::VehicleAtomic obj,
	                                        components::VehicleMaterial mat) {
		                                    auto &color = colors_[i];
		                                    if (color.first == 0) {
			                                    memcpy(color.second.data(), this->primaryColor_, sizeof color.second);
			                                    mat.setColor(floatColorToCRGBA(color.second));
		                                    }
	                                    });
}

void Vehicle::setEditorSecondaryColor() {
	components::extractComObjMatVehicle(this->editorVehicle_.value(),
	                                    [&](int i, components::VehicleComponent comp, components::VehicleAtomic obj,
	                                        components::VehicleMaterial mat) {
		                                    auto &color = colors_[i];
		                                    if (color.first == 1) {
			                                    memcpy(color.second.data(), this->secondaryColor_, sizeof color.second);
			                                    mat.setColor(floatColorToCRGBA(color.second));
		                                    }
	                                    });
}

void Vehicle::spawnEditorVehicle(const bool recolor) {
	if (editorVehicle_.has_value())
		this->deleteEditorVehicle();

	this->editorVehicle_ = spawnVehicle(recolor);
	this->editorVehicle_.value()->m_bUsesCollision = 0;
	this->editorVehicle_.value()->m_nPhysicalFlags.bBulletProof = 1;
	this->editorVehicle_.value()->m_nPhysicalFlags.bCollisionProof = 1;
	this->editorVehicle_.value()->m_nPhysicalFlags.bExplosionProof = 1;
	this->editorVehicle_.value()->m_nPhysicalFlags.bFireProof = 1;
	this->editorVehicle_.value()->m_nPhysicalFlags.bMeleeProof = 1;
	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(this->editorVehicle_.value()),
	                                                               1);

	updateLocation();
}

extern bool restart;

void Vehicle::deleteEditorVehicle() {
	if (this->editorVehicle_.has_value() && !restart) {
		const auto vehicle = this->editorVehicle_.value();
		if (CPools::ms_pVehiclePool->IsObjectValid(vehicle)) {
			const int vehicleRef = CPools::GetVehicleRef(vehicle);
			Command<Commands::DELETE_CAR>(vehicleRef);
		}
		this->editorVehicle_ = std::nullopt;
	}
}

void Vehicle::spawnProjectEntity() {
	if (projectVehicle_.has_value())
		this->deleteProjectEntity();

	this->projectVehicle_ = spawnVehicle(false);

	updateLocation();
}

void Vehicle::deleteProjectEntity() {
	if (this->projectVehicle_.has_value() && !restart) {
		if (const auto vehicle = this->projectVehicle_.value(); CPools::ms_pVehiclePool->IsObjectValid(vehicle)) {
			if (FindPlayerPed()->m_pVehicle == vehicle) {
				const auto destination = FindPlayerPed()->m_pVehicle->GetPosition() + CVector(0, 0, 1);
				FindPlayerPed()->Teleport(destination, false);
			}
			const int vehicleRef = CPools::GetVehicleRef(this->projectVehicle_.value());
			Command<Commands::DELETE_CAR>(vehicleRef);
		}
		this->projectVehicle_ = std::nullopt;
	}
}

void Vehicle::takeUpgrades() {
	std::array<int, 15> newUpgrades;
	newUpgrades.fill(-1);
	if (this->editorVehicle_.has_value()) {
		ranges::copy(this->editorVehicle_.value()->m_anUpgrades,
		             std::begin(newUpgrades));
		this->paintjob_ = this->editorVehicle_.value()->GetRemapIndex();
		if (newUpgrades != upgrades_) {
			upgrades_ = newUpgrades;
			recolorVehicle(true, this->editorVehicle_.value());
		}
	}
}

void Vehicle::restoreUpgrades(CVehicle *newVehicle, bool recolor) {
	Tasker::getInstance().addTask("restoreUpgrade",
	                              [](Vehicle *this_, CVehicle *newVehicle, const bool recolor) -> ktwait {
		                              for (int i = 0; i < this_->upgrades_.size(); ++i) {
			                              if (const auto upgrade = this_->upgrades_[i]; upgrade != -1) {
				                              if (!CStreaming::HasVehicleUpgradeLoaded(upgrade)) {
					                              CStreaming::RequestVehicleUpgrade(upgrade, GAME_REQUIRED);
					                              CStreaming::LoadAllRequestedModels(false);
					                              while (!CStreaming::HasVehicleUpgradeLoaded(upgrade)) {
						                              co_await 1;
					                              }
				                              }
				                              int handle;
				                              Command<Commands::ADD_VEHICLE_MOD>(newVehicle, upgrade, &handle);
			                              }
		                              }
		                              this_->recolorVehicle(recolor, newVehicle);
	                              }, this, newVehicle, recolor);
}
