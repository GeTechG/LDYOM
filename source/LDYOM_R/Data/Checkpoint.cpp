﻿#include "Checkpoint.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"
#include "CTheScripts.h"
#include <C3dMarkers.h>
#include <CCheckpoints.h>
#include <CWorld.h>

#include "Events.h"

using namespace plugin;

std::optional<int> Checkpoint::spawnCheckpoint() {
	const int scriptCheckpoint = CTheScripts::AddScriptCheckpoint(this->pos_[0], this->pos_[1], this->pos_[2], this->pos_[0], this->pos_[1], this->pos_[2], this->radius_, this->checkpointType_);
	if (scriptCheckpoint == 0)
		return std::nullopt;
	return scriptCheckpoint;
}

void Checkpoint::drawSphere() {
	if (this->rerender) {
		this->rerender = false;
		return;
	}
	CVector pos = { this->pos_[0], this->pos_[1], this->pos_[2] };
	const auto red = static_cast<unsigned char>(color_[0] * 255);
	const auto green = static_cast<unsigned char>(color_[1] * 255);
	const auto blue = static_cast<unsigned char>(color_[2] * 255);
	const auto alpha = static_cast<unsigned char>(color_[3] * 255);
	if (this->type == 0) {
		C3dMarkers::PlaceMarkerSet(reinterpret_cast<unsigned>(this), static_cast<unsigned short>(this->checkpointType_), pos, this->radius_, red, green, blue, alpha, static_cast<unsigned short>(this->pulsePeriod), this->pulseFraction, static_cast<short>(this->rotateRate));
	} else {
		C3dMarkers::PlaceMarkerCone(reinterpret_cast<unsigned>(this), pos, this->radius_, red, green, blue, alpha, static_cast<unsigned short>(this->pulsePeriod), this->pulseFraction, static_cast<short>(this->rotateRate), false);
	}
}

bool Checkpoint::existProjectEntity() {
	return this->getProjectCheckpoint().has_value() || this->getProjectSphere().has_value();
}

CVector Checkpoint::getProjectEntityPosition() {
	if (this->type == 0) {
		return {this->pos_[0], this->pos_[1], this->pos_[2] };
	}
	return this->getProjectCheckpoint().value()->m_vecPosition;
}

Checkpoint::Checkpoint(const char* name, const CVector& pos): ObjectiveDependent(nullptr),
                                                              uuid_(boost::uuids::random_generator()()),
															  checkpointType_(1) {
	strlcpy(this->name_.data(), name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = CWorld::FindGroundZForCoord(pos.x, pos.y);
}

Checkpoint::Checkpoint(const Checkpoint& other): ObjectiveDependent{other},
                                                 INameable{other},
                                                 IPositionable{other},
                                                 IUuidable{other},
                                                 uuid_{boost::uuids::random_generator()()},
                                                 name_{other.name_},
                                                 pos_{other.pos_},
                                                 type{other.type},
                                                 radius_{other.radius_},
                                                 blipColor_{other.blipColor_},
                                                 color_{other.color_},
                                                 checkpointType_{other.checkpointType_},
                                                 pulsePeriod{other.pulsePeriod},
                                                 pulseFraction{other.pulseFraction},
                                                 rotateRate{other.rotateRate},
                                                 blipType_{other.blipType_},
                                                 blipSprite_{other.blipSprite_},
                                                 angle{other.angle} {
	strlcat(this->name_.data(), "C", sizeof this->name_);
}

Checkpoint& Checkpoint::operator=(const Checkpoint& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	name_ = other.name_;
	pos_ = other.pos_;
	type = other.type;
	radius_ = other.radius_;
	blipColor_ = other.blipColor_;
	color_ = other.color_;
	checkpointType_ = other.checkpointType_;
	pulsePeriod = other.pulsePeriod;
	pulseFraction = other.pulseFraction;
	rotateRate = other.rotateRate;
	blipType_ = other.blipType_;
	blipSprite_ = other.blipSprite_;
	angle = other.angle;
	return *this;
}

Checkpoint::~Checkpoint() {
	this->deleteEditorCheckpoint();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Checkpoint::getUuid() {
	return uuid_;
}

std::optional<CCheckpoint*> Checkpoint::getEditorCheckpoint() {
	if (this->editorCheckpoint_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->editorCheckpoint_.value(), 3);
		const auto actual = static_cast<CCheckpoint*>(CTheScripts::ScriptCheckpointArray[actualScriptThingIndex].field_4);
		if (actual)
			return actual;
	}
	return std::nullopt;
}

std::optional<CCheckpoint*> Checkpoint::getProjectCheckpoint() {
	if (this->projectCheckpoint_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->projectCheckpoint_.value(), 3);
		const auto actual = static_cast<CCheckpoint*>(CTheScripts::ScriptCheckpointArray[actualScriptThingIndex].field_4);
		if (actual)
			return actual;
	}
	return std::nullopt;
}

std::optional<unsigned>& Checkpoint::getEditorSphere() {
	return editorSphere;
}

std::optional<unsigned>& Checkpoint::getProjectSphere() {
	return projectSphere;
}

void Checkpoint::updateLocation() {
	const auto & editorCheckpoint = this->getEditorCheckpoint();
	if (editorCheckpoint.has_value()) {
		editorCheckpoint.value()->m_vecPosition = { this->pos_[0], this->pos_[1], this->pos_[2] };
		CCheckpoints::SetHeading(editorCheckpoint.value()->m_nIdentifier, this->angle);
	}
	const auto& projectCheckpoint = this->getProjectCheckpoint();
	if (projectCheckpoint.has_value()) {
		projectCheckpoint.value()->m_vecPosition = { this->pos_[0], this->pos_[1], this->pos_[2] };
		CCheckpoints::SetHeading(projectCheckpoint.value()->m_nIdentifier, this->angle);
	}
	this->rerender = true;
}

char* Checkpoint::getName() {
	return this->name_.data();
}

float* Checkpoint::getPosition() {
	return pos_.data();
}

int& Checkpoint::getType() {
	return type;
}

float& Checkpoint::getRadius() {
	return radius_;
}

int& Checkpoint::getBlipColor() {
	return blipColor_;
}

std::array<float, 4>& Checkpoint::getColor() {
	return color_;
}

int& Checkpoint::getCheckpointType() {
	return checkpointType_;
}

int& Checkpoint::getPulsePeriod() {
	return pulsePeriod;
}

float& Checkpoint::getPulseFraction() {
	return pulseFraction;
}

int& Checkpoint::getRotateRate() {
	return rotateRate;
}

int& Checkpoint::getBlipType() {
	return blipType_;
}

int& Checkpoint::getBlipSprite() {
	return blipSprite_;
}

float& Checkpoint::getAngle() {
	return angle;
}

void Checkpoint::spawnEditorCheckpoint() {
	this->deleteEditorCheckpoint();

	if (this->type == 0) {
		unsigned slot = reinterpret_cast<unsigned>(&this->editorSphere);
		Events::processScriptsEvent.AddAtId(slot, [this] {
			this->drawSphere();
		});
		this->editorSphere = slot;
	} else {
		this->editorCheckpoint_ = this->spawnCheckpoint();
	}

	this->spawnEditorBlip();
	updateLocation();
}

extern bool restart;
void Checkpoint::deleteEditorCheckpoint() {
	if (this->editorCheckpoint_.has_value()) {
		CTheScripts::RemoveScriptCheckpoint(this->editorCheckpoint_.value());
		this->editorCheckpoint_ = std::nullopt;
	}
	if (this->editorSphere.has_value()) {
		Events::processScriptsEvent.RemoveById(this->editorSphere.value());
		this->editorSphere = std::nullopt;
	}
	this->deleteEditorBlip();
}

void Checkpoint::spawnEditorBlip() {
	deleteEditorBlip();
	if (this->blipColor_ != 0 || this->blipType_ == 1)
		this->editorBlip = utils::createBlip(this->pos_.data(), this->blipType_, this->blipColor_, this->blipSprite_);
}

void Checkpoint::deleteEditorBlip() {
	if (this->editorBlip.has_value()) {
		if (this->editorBlip.has_value())
			Command<Commands::REMOVE_BLIP>(this->editorBlip.value());
		this->editorBlip = std::nullopt;
	}
}

void Checkpoint::spawnProjectBlip() {
	deleteProjectBlip();
	if (this->blipColor_ != 0 || this->blipType_ == 1)
		this->projectBlip = utils::createBlip(this->pos_.data(), this->blipType_, this->blipColor_, this->blipSprite_);
}

void Checkpoint::deleteProjectBlip() {
	if (this->projectBlip.has_value()) {
		if (this->projectBlip.has_value())
			Command<Commands::REMOVE_BLIP>(this->projectBlip.value());
		this->projectBlip = std::nullopt;
	}
}

void Checkpoint::spawnProjectEntity() {
	this->deleteProjectEntity();

	if (this->type == 0) {
		unsigned slot = reinterpret_cast<unsigned>(&this->projectSphere);
		Events::processScriptsEvent.AddAtId(slot, [this] {
			this->drawSphere();
			});
		this->projectSphere = slot;
	}
	else {
		this->projectCheckpoint_ = this->spawnCheckpoint();
	}

	this->spawnProjectBlip();
	updateLocation();
}

void Checkpoint::deleteProjectEntity() {
	if (this->projectCheckpoint_.has_value()) {
		CTheScripts::RemoveScriptCheckpoint(this->projectCheckpoint_.value());
		this->projectCheckpoint_ = std::nullopt;
	}
	if (this->projectSphere.has_value()) {
		Events::processScriptsEvent.RemoveById(this->projectSphere.value());
		this->projectSphere = std::nullopt;
	}
	this->deleteProjectBlip();
}