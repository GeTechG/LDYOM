#include "Vehicle.h"

#include <array>
#include <CPools.h>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"

using namespace plugin;

void Vehicle::recolorVehicle(const bool recolor, CVehicle* const newVehicle) {
	if (recolor) {
		colors_.clear();
		components::extractComObjMatVehicle(newVehicle, [&] (int i, components::VehicleComponent comp, components::VehicleAtomic obj, components::VehicleMaterial mat) {
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
			}
			else if (rgba.r == 0xFF && rgba.g == 0x00 && rgba.b == 0xAF) {
				color.Set(utils::getCarColorRgba(newVehicle->m_nSecondaryColor));
				secondaryColor_[0] = static_cast<float>(color.a) / 255.0f;
				secondaryColor_[1] = static_cast<float>(color.b) / 255.0f;
				secondaryColor_[2] = static_cast<float>(color.g) / 255.0f;
				secondaryColor_[3] = static_cast<float>(color.r) / 255.0f;
				type = 1;
			}
			else {
				type = 2;
			}
			array color_rgba{ static_cast<float>(rgba.r) / 255.0f,
				static_cast<float>(rgba.g) / 255.0f,
				static_cast<float>(rgba.b) / 255.0f,
				static_cast<float>(rgba.a) / 255.0f };
			colors_.emplace_back(type, color_rgba);
		});
	} else {
		components::extractComObjMatVehicle(newVehicle, [&](const int i, components::VehicleComponent comp, components::VehicleAtomic obj, components::VehicleMaterial mat) {
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
	if (std::strcmp(numberplate_, "") != 0)
		Command<Commands::CUSTOM_PLATE_FOR_NEXT_CAR>(model, numberplate_);
	Command<Commands::CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR>(model, numberplateCity_);

	int newVehicleHandle;
	Command<Commands::CREATE_CAR>(model, this->pos_[0], this->pos_[1], this->pos_[2], &newVehicleHandle);
	const auto newVehicle = CPools::GetVehicle(newVehicleHandle);

	CStreaming::SetMissionDoesntRequireModel(model);

	newVehicle->m_nVehicleFlags.bEngineOn = 0;
	newVehicle->m_fHealth = static_cast<float>(this->health_);

	recolorVehicle(recolor, newVehicle);

	return newVehicle;
}

Vehicle::Vehicle(const char* name, const CVector& pos, float headingAngle): ObjectiveDependent(nullptr),
																			uuid_(boost::uuids::random_generator()()), headingAngle_(headingAngle),
                                                                            modelId_(400), health_(1000),
                                                                            numberplateCity_(-1) {
	strlcpy(this->name_, name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Vehicle::Vehicle(const Vehicle& other): INameable{other},
                                        IPositionable{other}, uuid_(boost::uuids::random_generator()()),
                                        headingAngle_{other.headingAngle_},
                                        modelId_{other.modelId_},
                                        shouldNotDie_{other.shouldNotDie_},
                                        health_{other.health_},
                                        bulletproof_{other.bulletproof_},
                                        fireproof_{other.fireproof_},
                                        explosionproof_{other.explosionproof_},
                                        collisionproof_{other.collisionproof_},
                                        meleeproof_{other.meleeproof_},
                                        tiresVulnerability_{other.tiresVulnerability_},
                                        extendedColor_{other.extendedColor_},
                                        locked_{other.locked_},
                                        colors_{other.colors_},
                                        componentTypeA_{other.componentTypeA_},
                                        componentTypeB_{other.componentTypeB_},
                                        numberplateCity_{other.numberplateCity_} {
	strlcat(name_, other.name_, sizeof name_);
	strlcat(name_, "C", sizeof name_);
	memcpy(pos_, other.pos_, sizeof pos_);
	memcpy(primaryColor_, other.primaryColor_, sizeof primaryColor_);
	memcpy(secondaryColor_, other.secondaryColor_, sizeof secondaryColor_);
	strlcat(numberplate_, other.numberplate_, sizeof numberplate_);
}

Vehicle& Vehicle::operator=(const Vehicle& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	headingAngle_ = other.headingAngle_;
	modelId_ = other.modelId_;
	shouldNotDie_ = other.shouldNotDie_;
	health_ = other.health_;
	bulletproof_ = other.bulletproof_;
	fireproof_ = other.fireproof_;
	explosionproof_ = other.explosionproof_;
	collisionproof_ = other.collisionproof_;
	meleeproof_ = other.meleeproof_;
	tiresVulnerability_ = other.tiresVulnerability_;
	extendedColor_ = other.extendedColor_;
	locked_ = other.locked_;
	colors_ = other.colors_;
	componentTypeA_ = other.componentTypeA_;
	componentTypeB_ = other.componentTypeB_;
	numberplateCity_ = other.numberplateCity_;
	return *this;
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

char* Vehicle::getNumberplate() {
	return this->numberplate_;
}


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

char* Vehicle::getName() {
	return this->name_;
}

float* Vehicle::getPosition() {
	return pos_;
}

float& Vehicle::getHeadingAngle() {
	return headingAngle_;
}

void Vehicle::setEditorPrimaryColor() {
	components::extractComObjMatVehicle(this->editorVehicle_.value(), [&] (int i, components::VehicleComponent comp, components::VehicleAtomic obj, components::VehicleMaterial mat) {
		auto &color = colors_[i];
		if (color.first == 0) {
			memcpy(color.second.data(), this->primaryColor_, sizeof color.second);
			mat.setColor(floatColorToCRGBA(color.second));
		}
	});
}

void Vehicle::setEditorSecondaryColor() {
	components::extractComObjMatVehicle(this->editorVehicle_.value(), [&](int i, components::VehicleComponent comp, components::VehicleAtomic obj, components::VehicleMaterial mat) {
		auto& color = colors_[i];
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
	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(this->editorVehicle_.value()), 1);

	updateLocation();
}

extern bool restart;
void Vehicle::deleteEditorVehicle() {
	if (this->editorVehicle_.has_value() && !restart) {
		int vehicleRef = CPools::GetVehicleRef(this->editorVehicle_.value());
		Command<Commands::DELETE_CAR>(vehicleRef);
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
		int vehicleRef = CPools::GetVehicleRef(this->projectVehicle_.value());
		Command<Commands::DELETE_CAR>(vehicleRef);
		this->projectVehicle_ = std::nullopt;
	}
}
