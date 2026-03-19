#include "carrec_paths_binding.h"
#include <CPools.h>
#include <CVehicle.h>
#include <logger.h>
#include <utils/carrec_paths_service.h>
#include <uuid_wrap.h>

void register_carrec_paths_bindings(sol::state_view lua) {
	sol::table carrec_paths = lua.create_named_table("carrec_paths");

	// carrec_paths.get_paths() -> { {name, uuid, frame_count, duration}, ... }
	// Returns info about all available carrec paths.
	carrec_paths.set_function("get_paths", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		sol::table result = l.create_table();

		const auto& paths = CarrecPathsService::instance().getPaths();
		for (int i = 0; i < static_cast<int>(paths.size()); ++i) {
			const auto& path = paths[i];
			sol::table entry = l.create_table();
			entry["name"] = path.getName();
			entry["uuid"] = uuids::to_string(path.getId());
			entry["frame_count"] = static_cast<int>(path.getFrameCount());
			entry["duration"] = path.getDuration();
			result[i + 1] = entry;
		}
		return result;
	});

	// carrec_paths.start_playback(vehicle_ref, path_uuid_or_name, use_ai, looped) -> bool
	// Starts carrec playback on the vehicle identified by its GTA script handle (ref).
	// path_uuid_or_name: UUID string or display name of the path.
	// Returns true on success.
	carrec_paths.set_function("start_playback",
		[](int vehicleRef, const std::string& pathId, sol::optional<bool> useAI, sol::optional<bool> looped) -> bool {
			CVehicle* vehicle = CPools::GetVehicle(vehicleRef);
			if (!vehicle) {
				LDYOM_WARN("carrec_paths.start_playback: invalid vehicle ref {}", vehicleRef);
				return false;
			}

			CarrecPath* path = CarrecPathsService::instance().getPathByUuidString(pathId);

			// If not found by UUID, try by name
			if (!path) {
				for (auto& p : CarrecPathsService::instance().getPaths()) {
					if (p.getName() == pathId) {
						path = &p;
						break;
					}
				}
			}

			if (!path) {
				LDYOM_WARN("carrec_paths.start_playback: path not found '{}'", pathId);
				return false;
			}

			if (path->getFrameCount() == 0) {
				LDYOM_WARN("carrec_paths.start_playback: path '{}' has no frames", path->getName());
				return false;
			}

			CarrecPathsService::stopPlaybackRecordedCar(vehicle);
			CarrecPathsService::startPlaybackRecordedCar(vehicle, path->getFrames(), useAI.value_or(false),
			                                             looped.value_or(false));
			return true;
		});

	// carrec_paths.stop_playback(vehicle_ref)
	// Stops carrec playback on the vehicle.
	carrec_paths.set_function("stop_playback", [](int vehicleRef) {
		CVehicle* vehicle = CPools::GetVehicle(vehicleRef);
		if (!vehicle) {
			LDYOM_WARN("carrec_paths.stop_playback: invalid vehicle ref {}", vehicleRef);
			return;
		}
		CarrecPathsService::stopPlaybackRecordedCar(vehicle);
	});

	// carrec_paths.is_playing(vehicle_ref) -> bool
	// Returns true if the vehicle currently has an active carrec playback.
	carrec_paths.set_function("is_playing", [](int vehicleRef) -> bool {
		CVehicle* vehicle = CPools::GetVehicle(vehicleRef);
		if (!vehicle)
			return false;
		return CarrecPathsService::isPlaybackActive(vehicle);
	});
}
