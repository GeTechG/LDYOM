#include "Audio.h"

#include <array>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "ModelsService.h"
#include "ProjectsService.h"
#include "utils.h"
#include "../Data/CSoundSystem.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

std::vector<std::string> Audio::audioFilesList_{};

std::vector<std::string> audioFileExtensions = {".mp3", ".mp2", ".mp1", ".ogg", ".wav", ".aiff"};

std::optional<int> Audio::loadAudio() {
	const auto iterator = std::ranges::find(audioFilesList_, this->audioName);
	if (iterator != audioFilesList_.end()) {
		const auto &audioPath = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->directory /
			L"audio" / utils::stringToPathString(*iterator);

		int audioHandle;
		if (this->audio3D) {
			Command<0x0AC1>(audioPath.string().c_str(), &audioHandle); //load3dAudioStream
		} else {
			Command<0x0AAC>(audioPath.string().c_str(), &audioHandle); //loadAudioStream
		}

		if (audioHandle == 0)
			return std::nullopt;

		if (this->isLooped)
			Command<0x0AC0>(audioHandle, 1); //SET_AUDIO_STREAM_LOOPED

		return audioHandle;
	}
	return std::nullopt;
}

Audio::Audio(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                    uuid(boost::uuids::random_generator()()) {
	this->name = name;
	this->pos[0] = pos.x;
	this->pos[1] = pos.y;
	this->pos[2] = pos.z;
}

Audio Audio::copy() const {
	Audio audio(*this);
	audio.uuid = boost::uuids::random_generator()();
	audio.name += " (copy)";
	audio.editorAudio_ = std::nullopt;
	audio.editorAudioObject_ = std::nullopt;
	audio.projectAudio_ = std::nullopt;
	return audio;
}

Audio::~Audio() {
	this->deleteEditorAudio();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Audio::getUuid() {
	return uuid;
}

std::optional<int>& Audio::getEditorAudio() {
	return editorAudio_;
}

std::optional<int>& Audio::getProjectAudio() {
	return projectAudio_;
}

std::string& Audio::getAudioName() {
	return audioName;
}

bool& Audio::isAudio3D() {
	return audio3D;
}

int& Audio::getAttachType3d() {
	return attachType3d;
}

boost::uuids::uuid& Audio::getAttachUuid() {
	return attachUuid;
}

bool& Audio::isIsLooped() {
	return isLooped;
}

float& Audio::getVolume() {
	return volume;
}

void Audio::updateLocation() {
	if (this->editorAudioObject_.has_value()) {
		CWorld::Remove(this->editorAudioObject_.value());
		this->editorAudioObject_.value()->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
		this->editorAudioObject_.value()->UpdateRwFrame();
		CWorld::Add(this->editorAudioObject_.value());
	}
	if (this->editorAudio_.has_value() && this->audio3D) {
		switch (this->attachType3d) {
			case 1: {
				const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
				const int index = utils::indexByUuid(actors, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CHAR
					Command<0x0AC4>(this->editorAudio_.value(), actors.at(index)->getEditorPed().value());
				}
				break;
			}
			case 2: {
				const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                      getVehicles();
				const int index = utils::indexByUuid(vehicles, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CAR
					Command<0x0AC5>(this->editorAudio_.value(), vehicles.at(index)->getEditorVehicle().value());
				}
				break;
			}
			case 3: {
				const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                     getObjects();
				const int index = utils::indexByUuid(objects, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_OBJECT
					Command<0x0AC3>(this->editorAudio_.value(), objects.at(index)->getEditorObject().value());
				}
				break;
			}
			default: {
				//SET_PLAY_3D_AUDIO_STREAM_AT_COORDS
				Command<0x0AC2>(this->editorAudio_.value(), this->pos[0], this->pos[1], this->pos[2]);
				break;
			}
		}
	}
	if (this->projectAudio_.has_value() && this->audio3D) {
		switch (this->attachType3d) {
			case 1: {
				const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
				const int index = utils::indexByUuid(actors, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CHAR
					Command<0x0AC4>(this->projectAudio_.value(), actors.at(index)->getProjectPed().value());
				}
				break;
			}
			case 2: {
				const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                      getVehicles();
				const int index = utils::indexByUuid(vehicles, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CAR
					Command<0x0AC5>(this->projectAudio_.value(), vehicles.at(index)->getProjectVehicle().value());
				}
				break;
			}
			case 3: {
				const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                     getObjects();
				const int index = utils::indexByUuid(objects, this->attachUuid);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_OBJECT
					Command<0x0AC3>(this->projectAudio_.value(), objects.at(index)->getProjectObject().value());
				}
				break;
			}
			default: {
				//SET_PLAY_3D_AUDIO_STREAM_AT_COORDS
				Command<0x0AC2>(this->projectAudio_.value(), this->pos[0], this->pos[1], this->pos[2]);
				break;
			}
		}
	}
}

std::string& Audio::getName() {
	return this->name;
}

float* Audio::getPosition() {
	return pos.data();
}

void Audio::spawnEditorAudio() {
	if (editorAudio_.has_value() || editorAudioObject_.has_value())
		this->deleteEditorAudio();

	this->editorAudio_ = loadAudio();

	if (this->audio3D && this->attachType3d == 0) {
		CStreaming::RequestModel(2231, 0);
		CStreaming::LoadAllRequestedModels(false);

		this->editorAudioObject_ = CObject::Create(2231);
		this->editorAudioObject_.value()->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
		this->editorAudioObject_.value()->m_bUsesCollision = false;

		CWorld::Add(this->editorAudioObject_.value());
	}

	updateLocation();
}

extern bool restart;

void Audio::deleteEditorAudio() {
	if (this->editorAudioObject_.has_value() && !restart) {
		if (CPools::ms_pObjectPool->IsObjectValid(this->editorAudioObject_.value())) {
			const int objectRef = CPools::GetObjectRef(this->editorAudioObject_.value());
			Command<Commands::DELETE_OBJECT>(objectRef);
			this->editorAudioObject_ = std::nullopt;
		}
	}
	if (this->editorAudio_.has_value()) {
		//REMOVE_AUDIO_STREAM
		Command<0x0AAE>(this->editorAudio_.value());
		this->editorAudio_ = std::nullopt;
	}
}

void Audio::spawnProjectEntity() {
	if (projectAudio_.has_value())
		this->deleteProjectEntity();

	this->projectAudio_ = loadAudio();
	updateLocation();
	const auto &audioStream = reinterpret_cast<CLEO::CAudioStream*>(this->projectAudio_.value());
	audioStream->Play();
}

void Audio::deleteProjectEntity() {
	if (this->projectAudio_.has_value() && !restart) {
		//REMOVE_AUDIO_STREAM
		Command<0x0AAE>(this->projectAudio_.value());
		this->projectAudio_ = std::nullopt;
	}
}

std::vector<std::string>& Audio::getAudioFilesList() {
	return audioFilesList_;
}

void Audio::loadAudioFilesList() {
	const auto &projectDirectory = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->directory;
	if (!projectDirectory.empty()) {
		audioFilesList_ = utils::getFilenameList(projectDirectory, audioFileExtensions);
	}
}
