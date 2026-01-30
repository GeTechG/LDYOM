#include "vehicle_play_carrec_path.h"
#include "components/vehicle.h"
#include "entity.h"
#include <fa_icons.h>
#include <project_player.h>
#include <spdlog/spdlog.h>
#include <utils/carrec_paths_service.h>

void components::VehiclePlayCarrecPath::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<VehiclePlayCarrecPath>("VehiclePlayCarrecPathComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::VehiclePlayCarrecPath, cast, pathUuid, useAI, looped,
	                 playbackSpeed, autoStart, startPlayback, stopPlayback, isPlaying);
}

components::VehiclePlayCarrecPath::VehiclePlayCarrecPath()
	: Component(TYPE) {}

std::shared_ptr<components::Vehicle> components::VehiclePlayCarrecPath::getVehicleComponent() {
	if (!this->entity) {
		return nullptr;
	}
	return Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
}

bool components::VehiclePlayCarrecPath::startPlayback() {
	stopPlayback(); // Stop any existing playback

	// Get vehicle component
	auto vehicle = getVehicleComponent();
	if (!vehicle || !vehicle->handle) {
		spdlog::warn("VehiclePlayCarrecPath: Cannot start playback, vehicle not spawned");
		return false;
	}

	// Get path
	if (pathUuid.empty()) {
		spdlog::warn("VehiclePlayCarrecPath: No path assigned");
		return false;
	}

	auto* path = CarrecPathsService::instance().getPathByUuidString(pathUuid);
	if (!path) {
		spdlog::error("VehiclePlayCarrecPath: Path not found: {}", pathUuid);
		return false;
	}

	if (path->getFrameCount() == 0) {
		spdlog::error("VehiclePlayCarrecPath: Path '{}' has no frames", path->getName());
		return false;
	}

	// Start playback
	CarrecPathsService::startPlaybackRecordedCar(vehicle->handle.get(), path->getFrames(), useAI, looped);

	if (m_playbackSlot == -1) {
		spdlog::error("VehiclePlayCarrecPath: Failed to start playback");
		return false;
	}

	spdlog::info("VehiclePlayCarrecPath: Started playback of '{}' in slot {}", path->getName(), m_playbackSlot);
	return true;
}

void components::VehiclePlayCarrecPath::stopPlayback() {
	if (m_playbackSlot == -1) {
		return;
	}

	auto vehicle = getVehicleComponent();
	if (vehicle && vehicle->handle) {
		CarrecPathsService::stopPlaybackRecordedCar(vehicle->handle.get());
	}

	m_playbackSlot = -1;
}

void components::VehiclePlayCarrecPath::renderPathSelection() {
	auto& paths = CarrecPathsService::instance().getPaths();

	// Find current path
	auto currentPathOpt = uuids::uuid::from_string(pathUuid);
	auto currentPath = std::find_if(paths.begin(), paths.end(), [&](const auto& path) {
		return currentPathOpt.has_value() && path.getId() == currentPathOpt.value();
	});

	const char* previewText = (currentPath != paths.end()) ? currentPath->getName().c_str() : tr("select_path").c_str();

	if (ImGui::BeginCombo("##pathSelect", previewText)) {
		for (size_t i = 0; i < paths.size(); ++i) {
			const auto& path = paths[i];
			const bool isSelected = (currentPath != paths.end() && &path == &(*currentPath));

			if (ImGui::Selectable(path.getName().c_str(), isSelected)) {
				pathUuid = uuids::to_string(path.getId());
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void components::VehiclePlayCarrecPath::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;
	const float labelPos = availableWidth * 0.45f;

	// Path selection
	ImGui::Text("%s", tr("path").c_str());
	ImGui::SameLine(labelPos);
	ImGui::SetNextItemWidth(-1.f);
	renderPathSelection();

	ImGui::Spacing();

	// Options
	ImGui::Text("%s", tr("use_ai").c_str());
	ImGui::SameLine(labelPos);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##useAI", &useAI);

	ImGui::Text("%s", tr("looped").c_str());
	ImGui::SameLine(labelPos);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##looped", &looped);

	ImGui::Text("%s", tr("auto_start").c_str());
	ImGui::SameLine(labelPos);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##autoStart", &autoStart);

	ImGui::Text("%s", tr("playback_speed").c_str());
	ImGui::SameLine(labelPos);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##playbackSpeed", &playbackSpeed, 0.1f, 0.1f, 5.0f, "%.1fx");
}

nlohmann::json components::VehiclePlayCarrecPath::to_json() const {
	nlohmann::json j = Component::to_json();
	j["pathUuid"] = pathUuid;
	j["useAI"] = useAI;
	j["looped"] = looped;
	j["playbackSpeed"] = playbackSpeed;
	j["autoStart"] = autoStart;
	return j;
}

void components::VehiclePlayCarrecPath::from_json(const nlohmann::json& j) {
	Component::from_json(j);

	if (j.contains("pathUuid"))
		pathUuid = j["pathUuid"].get<std::string>();
	if (j.contains("useAI"))
		useAI = j["useAI"].get<bool>();
	if (j.contains("looped"))
		looped = j["looped"].get<bool>();
	if (j.contains("playbackSpeed"))
		playbackSpeed = j["playbackSpeed"].get<float>();
	if (j.contains("autoStart"))
		autoStart = j["autoStart"].get<bool>();
}

void components::VehiclePlayCarrecPath::onStart() {
	Component::onStart();

	if (IS_PLAYING) {
		auto vehicle = getVehicleComponent();
		if (vehicle) {
			// Connect to vehicle spawned signal
			m_vehicleSpawnedConnection = std::make_optional(vehicle->onSpawned.connect([this]() {
				if (autoStart) {
					startPlayback();
				}
			}));

			// If vehicle is already spawned, start immediately
			if (vehicle->handle && autoStart) {
				startPlayback();
			}
		}
	}
}

void components::VehiclePlayCarrecPath::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);

	if (!IS_PLAYING || !isPlaying()) {
		return;
	}

	// Check if playback is still active
	auto vehicle = getVehicleComponent();
	if (!vehicle || !vehicle->handle) {
		m_playbackSlot = -1;
		return;
	}

	bool stillActive = CarrecPathsService::isPlaybackActive(vehicle->handle.get());
	if (!stillActive) {
		spdlog::debug("VehiclePlayCarrecPath: Playback completed");
		m_playbackSlot = -1;

		// Handle looping
		if (looped && !pathUuid.empty()) {
			spdlog::debug("VehiclePlayCarrecPath: Restarting looped playback");
			startPlayback();
		}
	}
}

void components::VehiclePlayCarrecPath::onReset() {
	Component::onReset();
	stopPlayback();
	m_vehicleSpawnedConnection.reset();
}

Dependencies components::VehiclePlayCarrecPath::getDependencies() { return Dependencies{{Vehicle::TYPE}, true}; }
