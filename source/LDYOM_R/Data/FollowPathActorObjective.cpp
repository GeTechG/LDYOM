#include "FollowPathActorObjective.h"

#include <CTimer.h>
#include <extensions/ScriptCommands.h>

#include "Actor.h"
#include "EditByPlayerService.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

FollowPathActorObjective::FollowPathActorObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.follow_path_actor"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void FollowPathActorObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);

	IncorrectHighlight(indexActor == -1, [&] {
		utils::Combo(local.get("entities.actor").c_str(), &this->actorUuid_, indexActor, actors.size(),
		             [&actors](const int i) {
			             return actors.at(i)->getName();
		             }, [&actors](const int i) {
			             return actors.at(i)->getUuid();
		             });
	});

	ImGui::SliderInt(local.get("follow_path_actor_objective.move_type").c_str(), &this->moveType_, 0, 2,
	                 local.getArray("follow_path_actor_objective.move_states").at(this->moveType_).c_str());
	ImGui::SliderInt(local.get("follow_path_actor_objective.path_type").c_str(), &this->pathType_, 0, 2,
	                 local.getArray("follow_path_actor_objective.path_types").at(this->pathType_).c_str());
	if (this->pathType_ > 0) {
		ImGui::DragFloat(local.get("follow_path_actor_objective.execute_time").c_str(), &this->executeTime_, 0.1f, 0.f,
		                 FLT_MAX);
	}
	ImGui::Checkbox(local.get("general.wait_end").c_str(), &this->waitEnd);
	if (ImGui::Button(local.get("general.record_path").c_str())) {
		EditByPlayerService::getInstance().editByPlayerActorPath(this->path_);
	}
}

int moveState[] = {4, 6, 7};

ktwait FollowPathActorObjective::execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	auto followPathFunc = [](FollowPathActorObjective *_this, Actor *actor) -> ktwait {
		auto execute = true;
		int step = 1;
		int index = 0;
		const auto startTime = CTimer::m_snTimeInMilliseconds;
		while (execute) {
			float x = _this->getPath().at(index)[0];
			float y = _this->getPath().at(index)[1];
			float z = _this->getPath().at(index)[2];
			Command<Commands::TASK_GO_STRAIGHT_TO_COORD>(actor->getProjectPed().value(), x, y, z,
			                                             moveState[_this->getMoveType()], -1);

			auto timeCondition = _this->getPathType() == 0 || CTimer::m_snTimeInMilliseconds - startTime < static_cast<
				unsigned>(_this->getExecuteTime() * 1000.f);
			const auto reachDistance = _this->getMoveType() == 0 ? 0.5f : 1.f;
			while (DistanceBetweenPoints(CVector(x, y, z), actor->getProjectPed().value()->GetPosition()) >
				reachDistance && timeCondition) {
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

	if (this->waitEnd)
		co_await followPathFunc(this, actor);
	else
		tasklist.add_task(followPathFunc, this, actor);

	co_return;
}

int& FollowPathActorObjective::getMoveType() {
	return moveType_;
}

int& FollowPathActorObjective::getPathType() {
	return pathType_;
}

float& FollowPathActorObjective::getExecuteTime() {
	return executeTime_;
}

std::vector<std::array<float, 3>>& FollowPathActorObjective::getPath() {
	return path_;
}
