#include "Audio.h"

#include <array>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"
#include "ProjectsService.h"
#include "../Data/CSoundSystem.h"

using namespace plugin;

std::vector<std::string> Audio::audioFilesList_{};

std::vector<std::string> audioFileExtensions = {".mp3", ".mp2", ".mp1", ".ogg", ".wav", ".aiff"};

std::optional<int> Audio::loadAudio() {
	const auto iterator = ranges::find(audioFilesList_, this->audioName_);
	if (iterator != audioFilesList_.end()) {
		const auto &audioPath = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->directory / L"Audio" / utils::stringToPathString(iterator->c_str());

		int audioHandle;
		if (this->audio3D_) {
			Command<0x0AC1>(audioPath.string().c_str(), &audioHandle); //load3dAudioStream
		}
		else {
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

Audio::Audio(const char* name, const CVector& pos): ObjectiveDependent(nullptr),
                                                    uuid_(boost::uuids::random_generator()()) {
	strlcpy(this->name_.data(), name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Audio::Audio(const Audio& other): ObjectiveDependent{other},
                                  INameable{other},
                                  IPositionable{other},
                                  IUuidable{other},
								  uuid_(boost::uuids::random_generator()()),
                                  name_{other.name_},
                                  pos_{other.pos_},
                                  audioName_{other.audioName_},
                                  audio3D_{other.audio3D_},
                                  attachType3d_{other.attachType3d_},
								  attachUuid_{ other.attachUuid_ },
								  volume_{other.volume_},
                                  isLooped{other.isLooped} {
	strlcat(name_.data(), "C", sizeof name_);
}

Audio& Audio::operator=(const Audio& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	name_ = other.name_;
	pos_ = other.pos_;
	audioName_ = other.audioName_;
	audio3D_ = other.audio3D_;
	attachType3d_ = other.attachType3d_;
	attachUuid_ = other.attachUuid_;
	isLooped = other.isLooped;
	volume_ = other.volume_;
	return *this;
}

Audio::~Audio() {
	this->deleteEditorAudio();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Audio::getUuid() {
	return uuid_;
}

std::optional<int>& Audio::getEditorAudio() {
	return editorAudio_;
}

std::optional<int>& Audio::getProjectAudio() {
	return projectAudio_;
}

std::string& Audio::getAudioName() {
	return audioName_;
}

bool& Audio::isAudio3D() {
	return audio3D_;
}

int& Audio::getAttachType3d() {
	return attachType3d_;
}

boost::uuids::uuid& Audio::getAttachUuid() {
	return attachUuid_;
}

bool& Audio::isIsLooped() {
	return isLooped;
}

float& Audio::getVolume() {
	return volume_;
}

void Audio::updateLocation() {
	if (this->editorAudioObject_.has_value()) {
		CWorld::Remove(this->editorAudioObject_.value());
		this->editorAudioObject_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorAudioObject_.value()->UpdateRwFrame();
		CWorld::Add(this->editorAudioObject_.value());
	}
	if (this->editorAudio_.has_value() && this->audio3D_) {
		switch (this->attachType3d_) {
			case 1: {
				const auto& actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
				const int index = utils::indexByUuid(actors, this->attachUuid_);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CHAR
					Command<0x0AC4>(this->editorAudio_.value(), actors.at(index).get()->getEditorPed().value());
				}
				break;
			}
			case 2: {
				const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
				const int index = utils::indexByUuid(vehicles, this->attachUuid_);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_CAR
					Command<0x0AC5>(this->editorAudio_.value(), vehicles.at(index).get()->getEditorVehicle().value());
				}
				break;
			}
			case 3: {
				const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
				const int index = utils::indexByUuid(objects, this->attachUuid_);
				if (index != -1) {
					//SET_PLAY_3D_AUDIO_STREAM_AT_OBJECT
					Command<0x0AC3>(this->editorAudio_.value(), objects.at(index).get()->getEditorObject().value());
				}
				break;
			}
			default: {
				//SET_PLAY_3D_AUDIO_STREAM_AT_COORDS
				Command<0x0AC2>(this->editorAudio_.value(), this->pos_[0], this->pos_[1], this->pos_[2]);
				break;
			}
		}
	}
	if (this->projectAudio_.has_value() && this->audio3D_) {
		switch (this->attachType3d_) {
		case 1: {
			const auto& actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
			const int index = utils::indexByUuid(actors, this->attachUuid_);
			if (index != -1) {
				//SET_PLAY_3D_AUDIO_STREAM_AT_CHAR
				Command<0x0AC4>(this->projectAudio_.value(), actors.at(index).get()->getProjectPed().value());
			}
			break;
		}
		case 2: {
			const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
			const int index = utils::indexByUuid(vehicles, this->attachUuid_);
			if (index != -1) {
				//SET_PLAY_3D_AUDIO_STREAM_AT_CAR
				Command<0x0AC5>(this->projectAudio_.value(), vehicles.at(index).get()->getProjectVehicle().value());
			}
			break;
		}
		case 3: {
			const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
			const int index = utils::indexByUuid(objects, this->attachUuid_);
			if (index != -1) {
				//SET_PLAY_3D_AUDIO_STREAM_AT_OBJECT
				Command<0x0AC3>(this->projectAudio_.value(), objects.at(index).get()->getProjectObject().value());
			}
			break;
		}
		default: {
			//SET_PLAY_3D_AUDIO_STREAM_AT_COORDS
			Command<0x0AC2>(this->projectAudio_.value(), this->pos_[0], this->pos_[1], this->pos_[2]);
			break;
		}
		}
	}
}

char* Audio::getName() {
	return this->name_.data();
}

float* Audio::getPosition() {
	return pos_.data();
}

void Audio::spawnEditorAudio() {
	if (editorAudio_.has_value() || editorAudioObject_.has_value())
		this->deleteEditorAudio();

	this->editorAudio_ = loadAudio();

	if (this->audio3D_ && this->attachType3d_ == 0) {
		CStreaming::RequestModel(2231, 0);
		CStreaming::LoadAllRequestedModels(false);

		this->editorAudioObject_ = CObject::Create(2231);
		this->editorAudioObject_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorAudioObject_.value()->m_bUsesCollision = false;

		CWorld::Add(this->editorAudioObject_.value());
	}

	updateLocation();
}

extern bool restart;
void Audio::deleteEditorAudio() {
	if (this->editorAudioObject_.has_value() && !restart) {
		int objectRef = CPools::GetObjectRef(this->editorAudioObject_.value());
		Command<Commands::DELETE_OBJECT>(objectRef);
		this->editorAudioObject_ = std::nullopt;
	}
	if (this->editorAudio_.has_value()) {
		//REMOVE_AUDIO_STREAM
		Command<0x0AAE>(this->editorAudio_.value());
		this->editorAudio_ = std::nullopt;
	}
}

void Audio::spawnProjectEntity() {
	if (this->projectAudio_.has_value()) {
		updateLocation();
		const auto & audioStream = reinterpret_cast<CLEO::CAudioStream*>(this->projectAudio_.value());
		audioStream->Play();
	}
}

void Audio::deleteProjectEntity() {
	if (this->projectAudio_.has_value() && !restart) {
		//REMOVE_AUDIO_STREAM
		Command<0x0AAE>(this->projectAudio_.value());
		this->projectAudio_ = std::nullopt;
	}
}

void Audio::preloadProjectAudio() {
	if (this->projectAudio_.has_value())
		this->deleteProjectEntity();

	this->projectAudio_ = loadAudio();
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
