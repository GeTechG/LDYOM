#include "carrec_paths_service.h"
#include "logger.h"
#include <fmt/core.h>
#include <projects_manager.h>

using json = nlohmann::json;

void CarrecPathsService::savePaths() const {
	const auto carrecDirectory =
		std::filesystem::path(ProjectsManager::instance().getCurrentProject().value()->path) / "carrec";

	if (std::error_code error; !remove_all(carrecDirectory, error)) {
		if (error.value() != 0)
			LDYOM_INFO("Failed to delete the project directory, error: {}", error.message());
	}

	if (std::error_code error; !create_directory(carrecDirectory, error)) {
		if (error.value() != 0)
			LDYOM_INFO("Failed to create the project directory, error: {}", error.message());
		return;
	}

	// save carrec paths
	for (const auto& carrecPath : this->paths) {
		std::ofstream file(carrecDirectory / fmt::format("{}.json", carrecPath.getName()));
		auto j = carrecPath.to_json();
		file << j.dump(-1, ' ', false, json::error_handler_t::ignore);
		file.close();
	}
}

void CarrecPathsService::loadPaths() {
	this->paths.clear();

	const auto carrecDirectory =
		std::filesystem::path(ProjectsManager::instance().getCurrentProject().value()->path) / "carrec";
	if (!exists(carrecDirectory)) {
		if (std::error_code error; !create_directory(carrecDirectory, error)) {
			if (error.value() != 0)
				LDYOM_INFO("Failed to create the project directory, error: {}", error.message());
			return;
		}
	}

	// Count files first to reserve space and avoid reallocation
	size_t fileCount = 0;
	for (const auto& entry : std::filesystem::directory_iterator(carrecDirectory)) {
		if (!entry.is_directory()) {
			const std::string extension = entry.path().extension().string();
			if (extension == ".json" || extension == ".rrr") {
				++fileCount;
			}
		}
	}
	this->paths.reserve(fileCount);

	for (const auto& entry : std::filesystem::directory_iterator(carrecDirectory)) {
		if (entry.is_directory())
			continue;

		try {
			std::string extension = entry.path().extension().string();
			if (extension == ".json") {
				std::ifstream file(entry.path());
				nlohmann::json j;
				file >> j;

				// Create object in place and load data directly into it
				CarrecPath::from_json(j, paths.emplace_back());

				file.close();
			} else if (extension == ".rrr") {
				std::ifstream file(entry.path(), std::ios::binary);
				auto* carrecPath = &paths.emplace_back();
				carrecPath->setName(entry.path().filename().stem().string());
				CVehicleStateEachFrame tempFrame;
				while (file.read(reinterpret_cast<char*>(&tempFrame), sizeof CVehicleStateEachFrame)) {
					carrecPath->getFrames().push_back(tempFrame);
				}

				if (!file.eof()) {
					LDYOM_INFO("Failed to read the file: {}", entry.path().string());
				}
			}
		} catch (const std::exception& e) {
			LDYOM_ERROR("Failed to load CarRec path from file '{}': {}", entry.path().string(), e.what());
		}
	}
}

CarrecPath* CarrecPathsService::createNewPath() {
	auto* carrecPath = &paths.emplace_back();
	carrecPath->setName(fmt::format("Path {:.4}", fmt::format("{:x}", reinterpret_cast<unsigned>(carrecPath))));
	return carrecPath;
}

std::vector<CarrecPath>& CarrecPathsService::getPaths() { return paths; }

bool CarrecPathsService::pathNameExists(const std::string& name) const {
	for (const auto& path : paths) {
		if (path.getName() == name) {
			return true;
		}
	}
	return false;
}

void CarrecPathsService::deletePath(const uuids::uuid& id) {
	auto it = std::find_if(paths.begin(), paths.end(), [&id](const CarrecPath& path) { return path.getId() == id; });
	if (it == paths.end()) {
		return;
	}
	const auto carrecDirectory =
		std::filesystem::path(ProjectsManager::instance().getCurrentProject().value()->path) / "carrec";
	const auto filePath = carrecDirectory / (fmt::format("{}.json", it->getName()));
	if (std::error_code error; !std::filesystem::remove(filePath, error)) {
		if (error.value() != 0)
			LDYOM_INFO("Failed to delete the carrec path file, error: {}", error.message());
	}

	paths.erase(it);
}

CarrecPath* CarrecPathsService::getPathByUuidString(const std::string& uuidStr) {
	auto uuidOpt = uuids::uuid::from_string(uuidStr);
	if (!uuidOpt.has_value()) {
		return nullptr;
	}
	const auto& uuid = uuidOpt.value();
	auto it =
		std::find_if(paths.begin(), paths.end(), [&uuid](const CarrecPath& path) { return path.getId() == uuid; });
	if (it != paths.end()) {
		return &(*it);
	}
	return nullptr;
}

void CarrecPathsService::startPlaybackRecordedCar(CVehicle* vehicle, std::vector<CVehicleStateEachFrame>& frames,
                                                  const bool useCarAI, const bool looped) {
	const auto getInactivePlaybackIndices = [] {
		return std::ranges::views::iota(0, TOTAL_VEHICLE_RECORDS) |
		       std::views::filter([](auto&& i) { return !CVehicleRecording::bPlaybackGoingOn[i]; });
	};
	const auto playbackId = *getInactivePlaybackIndices().begin();

	CVehicleRecording::pVehicleForPlayback[playbackId] = vehicle;
	CVehicleRecording::bPlaybackLooped[playbackId] = looped;
	CVehicleRecording::PlayBackStreamingIndex[playbackId] = 0;
	vehicle->RegisterReference(reinterpret_cast<CEntity**>(&CVehicleRecording::pVehicleForPlayback[playbackId]));
	CVehicleRecording::pPlaybackBuffer[playbackId] = frames.data();
	CVehicleRecording::PlaybackBufferSize[playbackId] = frames.size() * sizeof CVehicleStateEachFrame;
	CVehicleRecording::bUseCarAI[playbackId] = useCarAI;
	CVehicleRecording::PlaybackIndex[playbackId] = 0;
	CVehicleRecording::PlaybackRunningTime[playbackId] = 0.0f;
	CVehicleRecording::PlaybackSpeed[playbackId] = 1.0f;
	CVehicleRecording::bPlaybackGoingOn[playbackId] = true;
	CVehicleRecording::bPlaybackPaused[playbackId] = false;

	if (useCarAI) {
		vehicle->m_autoPilot.m_nCarMission = MISSION_FOLLOW_PRE_RECORDED_PATH;
		CVehicleRecording::SetRecordingToPointClosestToCoors(playbackId, vehicle->GetPosition());
	} else {
		vehicle->m_nPhysicalFlags.bDisableCollisionForce = static_cast<unsigned>(true);
		vehicle->m_nPhysicalFlags.bCollidable = static_cast<unsigned>(false);
	}
	vehicle->m_autoPilot.field_94 = static_cast<char>(playbackId);
}

void CarrecPathsService::stopPlaybackRecordedCar(CVehicle* vehicle) {
	const auto playbackId = CVehicleRecording::FindVehicleRecordingIndex(vehicle);
	if (playbackId != -1) {
		if (const auto vehiclePlayback = CVehicleRecording::pVehicleForPlayback[playbackId]) {
			vehiclePlayback->m_autoPilot.field_94 = -1;
			CVehicleRecording::pVehicleForPlayback[playbackId]->m_nPhysicalFlags.bDisableCollisionForce =
				static_cast<unsigned>(false);
		}
		CVehicleRecording::pVehicleForPlayback[playbackId] = nullptr;
		CVehicleRecording::pPlaybackBuffer[playbackId] = nullptr;
		CVehicleRecording::PlaybackBufferSize[playbackId] = 0;
		CVehicleRecording::bPlaybackGoingOn[playbackId] = false;
	}
}

bool CarrecPathsService::isPlaybackActive(CVehicle* vehicle) {
	const auto playbackId = CVehicleRecording::FindVehicleRecordingIndex(vehicle);
	return playbackId != -1 && CVehicleRecording::bPlaybackGoingOn[playbackId];
}
