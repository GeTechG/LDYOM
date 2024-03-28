#include "CarrecPathsService.h"
#include <fmt/core.h>

#include "imgui_notify.h"
#include "jsonUtils.h"
#include "ProjectsService.h"
#include "easylogging/easylogging++.h"

using json = nlohmann::json;

std::vector<CVehicleStateEachFrame>& CarrecPath::getPath() {
	return path;
}

Vehicle& CarrecPath::getVehicle() { return vehicle; }

std::string& CarrecPath::getName() { return name; }

boost::uuids::uuid& CarrecPath::getUuid() {
	return uuid;
}


void CarrecPathsService::savePaths(const std::filesystem::path &path) const {
	const auto carrecDirectory = path / "carrec";

	if (std::error_code error; !remove_all(carrecDirectory, error)) {
		if (error.value() != 0)
			CLOG(ERROR, "LDYOM") << "Failed to delete the project directory, error: " << error.message();
	}

	if (std::error_code error; !create_directory(carrecDirectory, error)) {
		if (error.value() != 0)
			CLOG(ERROR, "LDYOM") << "Failed to create the project directory, error: " << error.message();
		ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
		return;
	}

	//save carrec paths
	for (auto carrecPath : this->paths) {
		std::ofstream file(carrecDirectory / fmt::format("{}.json", carrecPath.getName()));
		nlohmann::json j;
		j["uuid"] = nlohmann::uuidToJson(carrecPath.getUuid());
		j["vehicle"] = carrecPath.getVehicle();
		j["path"] = carrecPath.getPath();
		file << j.dump(-1, ' ', false, json::error_handler_t::ignore);
		file.close();
	}
}

void CarrecPathsService::loadPaths(const std::filesystem::path &path) {
	const auto carrecDirectory = path / "carrec";
	if (!exists(carrecDirectory)) {
		if (std::error_code error; !create_directory(carrecDirectory, error)) {
			if (error.value() != 0)
				CLOG(ERROR, "LDYOM") << "Failed to create the project directory, error: " << error.message();
			ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			return;
		}
	}

	for (const auto &entry : std::filesystem::directory_iterator(carrecDirectory)) {
		if (entry.is_directory())
			continue;

		std::string extension = entry.path().extension().string();
		if (extension == ".json") {
			std::ifstream file(entry.path());
			nlohmann::json j;
			file >> j;
			auto *carrecPath = &paths.emplace_back();

			jsonToUuid(j["uuid"], carrecPath->getUuid());
			carrecPath->getName() = entry.path().filename().stem().string();
			if (!j.at("vehicle").is_null()) {
				carrecPath->getVehicle() = j.at("vehicle").get<Vehicle>();
			}
			j.at("path").get_to(carrecPath->getPath());

			file.close();
		} else if (extension == ".rrr") {
			std::ifstream file(entry.path(), std::ios::binary);
			auto *carrecPath = &paths.emplace_back();
			carrecPath->getName() = entry.path().filename().stem().string();
			CVehicleStateEachFrame tempFrame;
			while (file.read(reinterpret_cast<char*>(&tempFrame), sizeof CVehicleStateEachFrame)) {
				carrecPath->getPath().push_back(tempFrame);
			}

			if (!file.eof()) {
				CLOG(ERROR, "LDYOM") << "Failed to read the file: " << entry.path().string();
				ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			}
		}
	}
}

CarrecPath* CarrecPathsService::createNewPath() {
	auto *carrecPath = &paths.emplace_back();
	carrecPath->getName() = fmt::format("Path {:.4}", fmt::format("{:x}", reinterpret_cast<unsigned>(carrecPath)));
	return carrecPath;
}

std::vector<CarrecPath>& CarrecPathsService::getPaths() {
	return paths;
}

void CarrecPathsService::startPlaybackRecordedCar(CVehicle *vehicle, std::vector<CVehicleStateEachFrame> &frames,
                                                  const bool useCarAI, const bool looped) {
	const auto getInactivePlaybackIndices = [] {
		return std::ranges::views::iota(0, TOTAL_VEHICLE_RECORDS) | std::views::filter([](auto &&i) {
			return !CVehicleRecording::bPlaybackGoingOn[i];
		});
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

void CarrecPathsService::stopPlaybackRecordedCar(CVehicle *vehicle) {
	const auto playbackId = CVehicleRecording::FindVehicleRecordingIndex(vehicle);
	if (playbackId != -1) {
		if (const auto vehiclePlayback = CVehicleRecording::pVehicleForPlayback[playbackId]) {
			vehiclePlayback->m_autoPilot.field_94 = -1;
			CVehicleRecording::pVehicleForPlayback[playbackId]->
				m_nPhysicalFlags.bDisableCollisionForce = static_cast<unsigned>(false);
		}
		CVehicleRecording::pVehicleForPlayback[playbackId] = nullptr;
		CVehicleRecording::pPlaybackBuffer[playbackId] = nullptr;
		CVehicleRecording::PlaybackBufferSize[playbackId] = 0;
		CVehicleRecording::bPlaybackGoingOn[playbackId] = false;
	}
}
