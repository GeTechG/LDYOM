#include "FollowCarrecPathVehicleObjective.h"

#include "CarrecPathsService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utilsRender.h"

FollowCarrecPathVehicleObjective::FollowCarrecPathVehicleObjective(void *_new) : BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.follow_carrec_path_vehicle"));
	this->name += suffix;
}


void FollowCarrecPathVehicleObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	IncorrectHighlight(indexVehicle == -1, [&] {
		utils::Combo(local.get("entities.vehicle").c_str(), &this->vehicleUuid_, indexVehicle, vehicles.size(),
		             [&vehicles](const int i) {
			             return std::ref(vehicles.at(i)->getName());
		             }, [&vehicles](const int i) {
			             return vehicles.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (indexVehicle == -1) {
			if (!vehicles.empty()) {
				this->vehicleUuid_ = vehicles.back()->getUuid();
			}
		}
	}

	auto &carrecPaths = CarrecPathsService::getInstance().getPaths();
	const int indexPath = utils::indexByUuidValue(carrecPaths, this->pathUuid_);

	IncorrectHighlight(indexPath == -1, [&] {
		utils::Combo(local.get("carrec_path.path").c_str(), &this->pathUuid_, indexPath, carrecPaths.size(),
		             [&carrecPaths](const int i) {
			             return std::ref(carrecPaths.at(i).getName());
		             }, [&carrecPaths](const int i) {
			             return carrecPaths.at(i).getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (indexPath == -1) {
			if (!carrecPaths.empty()) {
				this->pathUuid_ = carrecPaths.back().getUuid();
			}
		}
	}

	utils::ToggleButton(local.get("follow_carrec_path_vehicle_objective.use_ai").c_str(), &this->useAI);
	utils::ToggleButton(local.get("animation.looped").c_str(), &this->looped_);
	ImGui::BeginDisabled(!this->looped_);
	ImGui::InputFloat(local.get("general.time").c_str(), &this->time_);
	ImGui::EndDisabled();
	utils::ToggleButton(local.get("general.wait_end").c_str(), &this->waitEnd_);

	if (indexVehicle != -1) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		auto position = vehicles.at(indexVehicle)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

ktwait FollowCarrecPathVehicleObjective::execute(Scene *scene, Vehicle *vehicle, Result &result,
                                                 ktcoro_tasklist &tasklist) {
	auto followPathFunc = [](const FollowCarrecPathVehicleObjective *_this, Vehicle *vehicle) -> ktwait {
		const auto vehicleRef = vehicle->getProjectVehicle().value();
		auto &carrecPaths = CarrecPathsService::getInstance().getPaths();
		auto &path = carrecPaths.at(utils::indexByUuidValue(carrecPaths, _this->pathUuid_));

		CarrecPathsService::startPlaybackRecordedCar(vehicleRef, path.getPath(), _this->useAI, _this->looped_);

		if (_this->looped_) {
			const auto startTime = CTimer::m_snTimeInMilliseconds;
			while (CTimer::m_snTimeInMilliseconds - startTime < static_cast<unsigned>(_this->time_ * 1000.f)) {
				co_await 1;
			}
			CarrecPathsService::stopPlaybackRecordedCar(vehicleRef);
		} else {
			while (CVehicleRecording::IsPlaybackGoingOnForCar(vehicleRef)) {
				co_await 1;
			}
		}
	};

	if (this->waitEnd_)
		co_await followPathFunc(this, vehicle);
	else
		tasklist.add_task(followPathFunc, this, vehicle);

	co_return;
}

boost::uuids::uuid& FollowCarrecPathVehicleObjective::getPathUuid() { return pathUuid_; }
bool& FollowCarrecPathVehicleObjective::isUseAI() { return useAI; }
bool& FollowCarrecPathVehicleObjective::isLooped() { return looped_; }
float& FollowCarrecPathVehicleObjective::getTime() { return time_; }
bool& FollowCarrecPathVehicleObjective::isWaitEnd() { return waitEnd_; }
