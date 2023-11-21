#include "Checkpoint.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include <C3dMarkers.h>
#include <CCheckpoints.h>
#include <CWorld.h>
#include "CTheScripts.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"

#include "Events.h"
#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "Scene.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

constexpr int CHECKPOINT_TYPES[] = {0, 1, 2, 5};

std::optional<int> Checkpoint::spawnCheckpoint() const {
	int scriptCheckpoint;
	Command<Commands::CREATE_CHECKPOINT>(this->checkpointType, this->pos[0], this->pos[1], this->pos[2],
	                                     this->pos[0], this->pos[1],
	                                     this->pos[2], this->radius, &scriptCheckpoint);
	if (scriptCheckpoint == 0)
		return std::nullopt;
	return scriptCheckpoint;
}

void Checkpoint::drawSphere() {
	if (this->rerender) {
		this->rerender = false;
		return;
	}
	CVector pos = {this->pos[0], this->pos[1], this->pos[2]};
	const auto red = static_cast<unsigned char>(color[0] * 255);
	const auto green = static_cast<unsigned char>(color[1] * 255);
	const auto blue = static_cast<unsigned char>(color[2] * 255);
	const auto alpha = static_cast<unsigned char>(color[3] * 255);
	if (this->type == 0) {
		C3dMarkers::PlaceMarkerSet(reinterpret_cast<unsigned>(this), CHECKPOINT_TYPES[this->checkpointType],
		                           pos, this->radius, red, green, blue, alpha, 2048, 0.1f, 228u);
	} else {
		C3dMarkers::PlaceMarkerCone(reinterpret_cast<unsigned>(this), pos, this->radius, red, green, blue, alpha, 2048,
		                            0.1f, 228u, false);
	}
}

bool Checkpoint::existProjectEntity() {
	return this->getProjectCheckpoint().has_value() || this->getProjectSphere().has_value();
}

CVector Checkpoint::getProjectEntityPosition() {
	if (this->type == 0) {
		return {this->pos[0], this->pos[1], this->pos[2]};
	}
	return this->getProjectCheckpoint().value()->m_vecPosition;
}

Checkpoint::Checkpoint(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                              uuid(boost::uuids::random_generator()()),
                                                              checkpointType(1) {
	this->name = name;
	this->pos[0] = pos.x;
	this->pos[1] = pos.y;
	this->pos[2] = CWorld::FindGroundZForCoord(pos.x, pos.y);
}

Checkpoint Checkpoint::copy() const {
	Checkpoint copy(*this);
	copy.uuid = boost::uuids::random_generator()();
	copy.name += " (copy)";

	return copy;
}

Checkpoint::~Checkpoint() {
	this->deleteEditorCheckpoint();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Checkpoint::getUuid() {
	return uuid;
}

std::optional<CCheckpoint*> Checkpoint::getEditorCheckpoint() {
	if (this->editorCheckpoint_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->editorCheckpoint_.value(), 3);
		const auto actual = static_cast<CCheckpoint*>(CTheScripts::ScriptCheckpointArray[actualScriptThingIndex].
			field_4);
		if (actual)
			return actual;
	}
	return std::nullopt;
}

std::optional<CCheckpoint*> Checkpoint::getProjectCheckpoint() {
	if (this->projectCheckpoint_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->projectCheckpoint_.value(), 3);
		const auto actual = static_cast<CCheckpoint*>(CTheScripts::ScriptCheckpointArray[actualScriptThingIndex].
			field_4);
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
	const auto &editorCheckpoint = this->getEditorCheckpoint();
	if (editorCheckpoint.has_value()) {
		editorCheckpoint.value()->m_vecPosition = {this->pos[0], this->pos[1], this->pos[2]};
		CCheckpoints::SetHeading(editorCheckpoint.value()->m_nIdentifier, this->angle);
	}
	const auto &projectCheckpoint = this->getProjectCheckpoint();
	if (projectCheckpoint.has_value()) {
		projectCheckpoint.value()->m_vecPosition = {this->pos[0], this->pos[1], this->pos[2]};
		CCheckpoints::SetHeading(projectCheckpoint.value()->m_nIdentifier, this->angle);
	}
	this->rerender = true;
}

std::string& Checkpoint::getName() {
	return this->name;
}

float* Checkpoint::getPosition() {
	return pos.data();
}

int& Checkpoint::getType() {
	return type;
}

float& Checkpoint::getRadius() {
	return radius;
}

int& Checkpoint::getBlipColor() {
	return blipColor;
}

std::array<float, 4>& Checkpoint::getColor() {
	return color;
}

int& Checkpoint::getCheckpointType() {
	return checkpointType;
}

int& Checkpoint::getBlipType() {
	return blipType;
}

int& Checkpoint::getBlipSprite() {
	return blipSprite;
}

float& Checkpoint::getAngle() {
	return angle;
}

void Checkpoint::spawnEditorCheckpoint() {
	this->deleteEditorCheckpoint();

	if (this->type == 0) {
		auto slot = reinterpret_cast<unsigned>(this) + 1;
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
	if (this->blipColor != 0 || this->blipType == 1)
		this->editorBlip = utils::createBlip(this->pos.data(), this->blipType, this->blipColor, this->blipSprite);
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
	if (this->blipColor != 0 || this->blipType == 1)
		this->projectBlip = utils::createBlip(this->pos.data(), this->blipType, this->blipColor, this->blipSprite);
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
		auto slot = reinterpret_cast<unsigned>(this) + 2;
		Events::processScriptsEvent.AddAtId(slot, [this] {
			this->drawSphere();
		});
		this->projectSphere = slot;
	} else {
		this->projectCheckpoint_ = this->spawnCheckpoint();
	}

	this->spawnProjectBlip();
	updateLocation();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		/*const auto onCheckpointSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
			"onCheckpointSpawn"].get_or_create<sol::table>();
		for (auto func : onCheckpointSpawn | std::views::values) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}
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

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	//if (scene.has_value() && tasklist != nullptr) {
	//	const auto onCheckpointDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
	//		"onCheckpointDelete"].get_or_create<sol::table>();
	//	for (auto [_, func] : onCheckpointDelete) {
	//		if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid); !result.valid()) {
	//			const sol::error err = result;
	//			CLOG(ERROR, "lua") << err.what();
	//		}
	//	}
	//}
}
