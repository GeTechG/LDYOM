#define IMGUI_DEFINE_MATH_OPERATORS
#include "audio.h"
#include "actor.h"
#include "extensions/ScriptCommands.h"
#include "object.h"
#include "objective_specific.h"
#include "vehicle.h"
#include <CStreaming.h>
#include <CTheScripts.h>
#include <entity.h>
#include <lua_define_type.h>
#include <paths.h>
#include <project_player.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils/utils_entities_selections.h>
#include <uuid_wrap.h>

// Initialize static members
std::vector<std::string> components::Audio::s_audioFilesList;
bool components::Audio::s_audioFilesLoaded = false;

void components::Audio::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Audio>("AudioComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Audio, cast, audioFileName, audio3D, attachType,
	                 attachEntityUuid, isLooped, volume, spawn, despawn);
}

components::Audio::Audio()
	: Component(TYPE) {}

inline nlohmann::json components::Audio::to_json() const {
	auto j = this->Component::to_json();
	j["audioFileName"] = audioFileName;
	j["audio3D"] = audio3D;
	j["attachType"] = attachType;
	j["attachEntityUuid"] = attachEntityUuid;
	j["isLooped"] = isLooped;
	j["volume"] = volume;
	return j;
}

void components::Audio::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("audioFileName").get_to(audioFileName);
	j.at("audio3D").get_to(audio3D);
	j.at("attachType").get_to(attachType);
	j.at("attachEntityUuid").get_to(attachEntityUuid);
	j.at("isLooped").get_to(isLooped);
	j.at("volume").get_to(volume);
}

void components::Audio::loadAudioFilesList() {
	s_audioFilesList.clear();

	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value())
		return;

	std::filesystem::path audioDir = std::filesystem::path(currentProject.value()->path) / "audio";

	if (!std::filesystem::exists(audioDir)) {
		std::filesystem::create_directories(audioDir);
		return;
	}

	const std::vector<std::string> extensions = {".mp3", ".mp2", ".mp1", ".ogg", ".wav", ".aiff"};

	for (const auto& entry : std::filesystem::directory_iterator(audioDir)) {
		if (entry.is_regular_file()) {
			auto ext = entry.path().extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
				s_audioFilesList.push_back(entry.path().filename().string());
			}
		}
	}

	s_audioFilesLoaded = true;
}

void components::Audio::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Audio file selector
	ImGui::Text(tr("audio_file").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##audioFile", audioFileName.c_str())) {
		if (s_audioFilesList.empty()) {
			ImGui::TextDisabled(tr("no_audio_files").c_str());
		} else {
			for (const auto& audioFile : s_audioFilesList) {
				if (ImGui::Selectable(audioFile.c_str(), audioFile == audioFileName)) {
					audioFileName = audioFile;
					dirty |= AudioFile;
				}
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(tr("audio_tooltip").c_str());
	}

	// Preview button
	if (!audioFileName.empty()) {
		ImGui::Text(tr("preview").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);

		bool isPlaying = editorAudioHandle.has_value();
		if (isPlaying) {
			int streamState = 0;
			plugin::Command<0x0AB9>(editorAudioHandle.value(), &streamState); // GET_AUDIO_STREAM_STATE
			isPlaying = (streamState == 1);                                   // 1 = playing
		}

		if (isPlaying) {
			if (ImGui::Button(tr("stop", ICON_FA_STOP).c_str(), ImVec2(-1, 0))) {
				if (editorAudioHandle.has_value()) {
					plugin::Command<0x0AAD>(editorAudioHandle.value(), 0); // SET_AUDIO_STREAM_STATE
					plugin::Command<0x0AAE>(editorAudioHandle.value());    // REMOVE_AUDIO_STREAM
					editorAudioHandle.reset();
				}
			}
		} else {
			if (ImGui::Button(tr("play", ICON_FA_PLAY).c_str(), ImVec2(-1, 0))) {
				spawn();
				if (editorAudioHandle.has_value()) {
					plugin::Command<0x0AAD>(editorAudioHandle.value(), 1); // SET_AUDIO_STREAM_STATE
				}
			}
		}
	}

	// Volume slider
	ImGui::Text(tr("volume").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderFloat("##volume", &volume, 0.0f, 2.0f, "%.2f")) {
		dirty |= Volume;
	}

	// Loop toggle
	ImGui::Text(tr("loop").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##loop", &isLooped)) {
		dirty |= Loop;
	}

	// 3D Audio toggle
	ImGui::Text(tr("audio_3d").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##audio3D", &audio3D)) {
		dirty |= Audio3D;
	}

	// 3D Audio settings
	if (audio3D) {
		// Attach type slider
		ImGui::Text(tr("attach_type").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		const char* attachTypes[] = {"Coordinates", "Actor", "Vehicle", "Object"};
		if (ImGui::SliderInt("##attachType", &attachType, 0, 3, attachTypes[attachType])) {
			dirty |= AttachType;
		}

		// Entity selector (if not coordinates)
		if (attachType != 0) {
			switch (attachType) {
				case 1: // Actor
					renderEntityByComponentSelection<Actor>(attachEntityUuid, availableWidth, 0.45f);
					break;
				case 2: // Vehicle
					renderEntityByComponentSelection<Vehicle>(attachEntityUuid, availableWidth, 0.45f);
					break;
				case 3: // Object
					renderEntityByComponentSelection<Object>(attachEntityUuid, availableWidth, 0.45f);
					break;
			}
		}
	}
}

void components::Audio::onStart() {
	Component::onStart();

	// Load audio files list on first use
	if (!s_audioFilesLoaded) {
		loadAudioFilesList();
	}

	// Set transform callbacks for position updates
	this->entity->setGetTransformCallbacks([this]() -> std::array<float, 3> { return {0.0f, 0.0f, 0.0f}; },
	                                       [this]() -> CQuaternion { return {}; });

	this->entity->setSetTransformCallbacks(
		[this](const std::array<float, 3>& position) {
			if (audio3D && attachType == 0) {
				dirty |= Position;
			}
		},
		[this](const CQuaternion rotation) {});

	if (!IS_PLAYING) {
		spawn();
	} else {
		auto objectiveSpecific = ObjectiveSpecific::cast(this->entity->getComponent(ObjectiveSpecific::TYPE));
		if (objectiveSpecific) {
			this->onSpawnedConnection = std::make_optional(objectiveSpecific->onSpawned.connect([this]() { spawn(); }));
			this->onDespawnedConnection =
				std::make_optional(objectiveSpecific->onDespawned.connect([this]() { despawn(); }));
		}
	}
}

void components::Audio::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);

	// Handle full respawn scenarios
	if (dirty & (AudioFile | Audio3D | AttachType | Loop)) {
		spawn();
	}
	// Handle position-only updates
	else if (dirty & Position) {
		updatePosition();
	}
	// Handle volume-only updates
	else if (dirty & Volume) {
		if (editorAudioHandle.has_value()) {
			plugin::Command<0x0ABC>(editorAudioHandle.value(), volume); // SET_AUDIO_STREAM_VOLUME
		}
		if (projectAudioHandle.has_value()) {
			plugin::Command<0x0ABC>(projectAudioHandle.value(), volume); // SET_AUDIO_STREAM_VOLUME
		}
	}

	dirty = None;
}

void components::Audio::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Audio::loadAudio() {
	if (audioFileName.empty())
		return;

	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value())
		return;

	std::filesystem::path absoluteAudioPath =
		std::filesystem::path(currentProject.value()->path) / "audio" / audioFileName;

	if (!std::filesystem::exists(absoluteAudioPath))
		return;

	// Convert to relative path from game directory
	std::filesystem::path audioPath = std::filesystem::relative(absoluteAudioPath, PLUGIN_PATH((char*)""));

	int handle = 0;
	std::string audioPathStr = audioPath.generic_string();

	if (audio3D) {
		plugin::Command<0x0AC1>(audioPathStr.c_str(), &handle); // LOAD_3D_AUDIO_STREAM
	} else {
		plugin::Command<0x0AAC>(audioPathStr.c_str(), &handle); // LOAD_AUDIO_STREAM
	}

	if (handle != 0) {
		// Set loop mode
		if (isLooped) {
			plugin::Command<0x0AC0>(handle, 1); // SET_AUDIO_STREAM_LOOPED
		}

		// Set volume
		plugin::Command<0x0ABC>(handle, volume); // SET_AUDIO_STREAM_VOLUME

		if (!IS_PLAYING) {
			editorAudioHandle = handle;
		} else {
			projectAudioHandle = handle;
		}
	}
}

void components::Audio::spawn() {
	despawn();

	if (audioFileName.empty())
		return;

	// Load audio stream
	loadAudio();

	// Create visual marker for 3D audio at coordinates
	if (audio3D && attachType == 0 && !IS_PLAYING) {
		constexpr int SPEAKER_MODEL_ID = 2231;
		CStreaming::RequestModel(SPEAKER_MODEL_ID, 0);
		CStreaming::LoadAllRequestedModels(false);

		if (CStreaming::ms_aInfoForModel[SPEAKER_MODEL_ID].m_nLoadState == 1) {
			auto& pos = entity->position;
			int newObject = 0;
			plugin::Command<plugin::Commands::CREATE_OBJECT>(SPEAKER_MODEL_ID, pos[0], pos[1], pos[2], &newObject);
			plugin::Command<plugin::Commands::SET_OBJECT_COLLISION>(newObject, false);

			CObject* obj = CPools::GetObject(newObject);
			if (obj) {
				m_visualMarker = std::shared_ptr<CObject>(obj, [](CObject* ptr) {
					if (ptr && !ScenesManager::instance().isRestartGame()) {
						auto ref = CPools::GetObjectRef(ptr);
						plugin::Command<plugin::Commands::DELETE_OBJECT>(ref);
					}
				});
			}
		}
	}

	// Set 3D audio position
	updatePosition();

	// Start playback in mission mode
	if (IS_PLAYING && projectAudioHandle.has_value()) {
		plugin::Command<0x0AAD>(projectAudioHandle.value(), 1); // SET_AUDIO_STREAM_STATE
		// new task for audio stream stopping when ended
		ProjectPlayer::instance().projectTasklist->add_task(
			[](Audio* audio) -> ktwait {
				int streamState = 0;
				plugin::Command<0x0AB9>(audio->projectAudioHandle.value(), &streamState); // GET_AUDIO_STREAM_STATE
				while (IS_PLAYING && streamState == 1) {                                  // 1 = playing
					co_await 100;
					if (audio->projectAudioHandle.has_value()) {
						plugin::Command<0x0AB9>(audio->projectAudioHandle.value(),
					                            &streamState); // GET_AUDIO_STREAM_STATE
					} else {
						break;
					}
				}
				audio->despawn();
			},
			this);
	}

	onSpawned();
}

void components::Audio::despawn() {
	onDespawned();

	// Remove visual marker
	if (m_visualMarker) {
		m_visualMarker.reset();
	}

	// Remove audio streams
	if (editorAudioHandle.has_value()) {
		plugin::Command<0x0AAE>(editorAudioHandle.value()); // REMOVE_AUDIO_STREAM
		editorAudioHandle.reset();
	}

	if (projectAudioHandle.has_value()) {
		plugin::Command<0x0AAE>(projectAudioHandle.value()); // REMOVE_AUDIO_STREAM
		projectAudioHandle.reset();
	}
}

Entity* components::Audio::findEntityByUuid(const std::string& uuid) const {
	if (uuid.empty())
		return nullptr;

	auto entities = IS_PLAYING ? ProjectPlayer::instance().getEntities() : std::vector<Entity*>([&]() {
		const auto& currentScene = ScenesManager::instance().getCurrentScene();
		std::vector<Entity*> result;
		for (const auto& ent : currentScene.entities) {
			result.push_back(ent.get());
		}
		return result;
	}());

	for (auto* ent : entities) {
		if (uuids::to_string(ent->id) == uuid) {
			return ent;
		}
	}
	return nullptr;
}

void components::Audio::updatePosition() {
	if (!audio3D)
		return;

	auto handle = IS_PLAYING ? projectAudioHandle : editorAudioHandle;
	if (!handle.has_value())
		return;

	switch (attachType) {
		case 0:
			{ // Coordinates
				auto& pos = entity->position;
				plugin::Command<0x0AC2>(handle.value(), pos[0], pos[1], pos[2]); // LINK_3D_AUDIO_TO_COORDS

				// Update visual marker position
				if (m_visualMarker && !IS_PLAYING) {
					// Proper matrix update following CObject::Teleport implementation
					m_visualMarker->Remove();

					// Update position in matrix or simple transform
					CMatrixLink* matrix = m_visualMarker->GetMatrix();
					if (matrix) {
						matrix->GetPosition().x = pos[0];
						matrix->GetPosition().y = pos[1];
						matrix->GetPosition().z = pos[2];
					} else {
						m_visualMarker->m_placement.m_vPosn.x = pos[0];
						m_visualMarker->m_placement.m_vPosn.y = pos[1];
						m_visualMarker->m_placement.m_vPosn.z = pos[2];
					}

					// Update RenderWare
					if (m_visualMarker->m_pRwObject) {
						if (matrix) {
							matrix->UpdateRW();
						}
						m_visualMarker->UpdateRwMatrix();
					}
					m_visualMarker->UpdateRwFrame();
					m_visualMarker->Add();
				}
				break;
			}
		case 1:
			{ // Actor
				auto* ent = findEntityByUuid(attachEntityUuid);
				if (ent) {
					auto actor = Actor::cast(ent->getComponent(Actor::TYPE));
					if (actor && actor->ped) {
						int pedRef = actor->getPedRef();
						if (pedRef != -1) {
							plugin::Command<0x0AC4>(handle.value(), pedRef); // LINK_3D_AUDIO_TO_PED
						}
					}
				}
				break;
			}
		case 2:
			{ // Vehicle
				auto* ent = findEntityByUuid(attachEntityUuid);
				if (ent) {
					auto vehicle = Vehicle::cast(ent->getComponent(Vehicle::TYPE));
					if (vehicle && vehicle->handle) {
						int vehicleRef = vehicle->getVehicleRef();
						if (vehicleRef != -1) {
							plugin::Command<0x0AC5>(handle.value(), vehicleRef); // LINK_3D_AUDIO_TO_CAR
						}
					}
				}
				break;
			}
		case 3:
			{ // Object
				auto* ent = findEntityByUuid(attachEntityUuid);
				if (ent) {
					auto object = Object::cast(ent->getComponent(Object::TYPE));
					if (object && object->handle) {
						int objectRef = object->getObjectRef();
						if (objectRef != -1) {
							plugin::Command<0x0AC3>(handle.value(), objectRef); // LINK_3D_AUDIO_TO_OBJECT
						}
					}
				}
				break;
			}
	}
}
