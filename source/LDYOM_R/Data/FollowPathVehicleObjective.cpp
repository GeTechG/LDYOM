#include "FollowPathVehicleObjective.h"

#include <CTimer.h>
#include <extensions/ScriptCommands.h>

#include "EditByPlayerService.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

FollowPathVehicleObjective::FollowPathVehicleObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.follow_path_vehicle"));
	this->name += suffix;
}

void FollowPathVehicleObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
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

	ImGui::SliderInt(local.get("follow_path_vehicle_objective.drive_type").c_str(), &this->driveType_, 0, 2,
	                 local.getArray("follow_path_vehicle_objective.drive_types").at(this->driveType_).c_str());
	ImGui::SliderInt(local.get("follow_path_actor_objective.path_type").c_str(), &this->pathType_, 0, 2,
	                 local.getArray("follow_path_actor_objective.path_types").at(this->pathType_).c_str());
	if (this->pathType_ > 0) {
		ImGui::DragFloat(local.get("follow_path_actor_objective.execute_time").c_str(), &this->executeTime_, 0.1f, 0.f,
		                 FLT_MAX);
	}
	ImGui::Checkbox(local.get("general.wait_end").c_str(), &this->waitEnd_);
	if (indexVehicle != -1) {
		if (ImGui::Button(local.get("general.record_path").c_str())) {
			EditByPlayerService::getInstance().editByPlayerVehiclePath(this->path_,
			                                                           vehicles.at(indexVehicle)->getModelId());
		}
	}
}

ktwait FollowPathVehicleObjective::execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	auto followPathFunc = [](FollowPathVehicleObjective *_this, Vehicle *vehicle) -> ktwait {
		auto execute = true;
		int step = 1;
		int index = 0;
		const auto startTime = CTimer::m_snTimeInMilliseconds;
		while (execute) {
			float x = _this->getPath().at(index)[0];
			float y = _this->getPath().at(index)[1];
			float z = _this->getPath().at(index)[2];
			switch (_this->getDriveType()) {
				case 0:
					Command<Commands::CAR_GOTO_COORDINATES>(vehicle->getProjectVehicle().value(), x, y, z);
					break;
				case 1:
					Command<Commands::CAR_GOTO_COORDINATES_ACCURATE>(vehicle->getProjectVehicle().value(), x, y, z);
					break;
				case 2:
					Command<Commands::CAR_GOTO_COORDINATES_RACING>(vehicle->getProjectVehicle().value(), x, y, z);
					break;
				default:
					break;
			}

			auto timeCondition = _this->getPathType() == 0 || CTimer::m_snTimeInMilliseconds - startTime < static_cast<
				unsigned>(_this->getExecuteTime() * 1000.f);
			while (DistanceBetweenPoints(CVector(x, y, z), vehicle->getProjectVehicle().value()->GetPosition()) > 2.f &&
				timeCondition) {
				timeCondition = _this->getPathType() == 0 || CTimer::m_snTimeInMilliseconds - startTime < static_cast<
					unsigned>(_this->getExecuteTime() * 1000.f);
				co_await 1;
			}

			index += step;

			if (!timeCondition)
				execute = false;

			switch (_this->getPathType()) {
				case 0:
					if (index == static_cast<int>(_this->getPath().size())) {
						execute = false;
					}
					break;
				case 1:
					if (index == static_cast<int>(_this->getPath().size())) {
						index = 0;
					}
					break;
				case 2:
					if (index == static_cast<int>(_this->getPath().size())) {
						step = -1;
						index -= 2;
					} else if (index < 0) {
						step = 1;
						index += 2;
					}
					break;
				default:
					break;
			}
		}
	};

	if (this->waitEnd_)
		co_await followPathFunc(this, vehicle);
	else
		tasklist.add_task(followPathFunc, this, vehicle);

	co_return;
}

int& FollowPathVehicleObjective::getDriveType() {
	return driveType_;
}

int& FollowPathVehicleObjective::getPathType() {
	return pathType_;
}

float& FollowPathVehicleObjective::getExecuteTime() {
	return executeTime_;
}

std::vector<std::array<float, 3>>& FollowPathVehicleObjective::getPath() {
	return path_;
}

bool& FollowPathVehicleObjective::isWaitEnd() { return waitEnd_; }
