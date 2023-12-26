#include "JumpToObjectiveObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

JumpToObjectiveObjective::JumpToObjectiveObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.jump_to_objective"));
	this->name += suffix;
}

boost::uuids::uuid& JumpToObjectiveObjective::getGoToObjectiveUuid() { return goToObjectiveUuid_; }
bool& JumpToObjectiveObjective::isCondition() { return condition_; }
int& JumpToObjectiveObjective::getConditionType() { return conditionType_; }
int& JumpToObjectiveObjective::getVarId() { return varId_; }
float& JumpToObjectiveObjective::getConditionValueNumber() { return conditionValueNumber_; }
bool& JumpToObjectiveObjective::isConditionValueBoolean() { return conditionValueBoolean_; }

void JumpToObjectiveObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &objectives = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives();
	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid_);

	IncorrectHighlight(jumpObjectiveIdx == -1 && this->condition_, [&] {
		utils::Combo(Localization::getInstance().get("objective.title").c_str(), &this->goToObjectiveUuid_,
		             jumpObjectiveIdx, objectives.size(), [&objectives](const int i) {
			             return std::ref(objectives.at(i)->getName());
		             }, [&objectives](const int i) {
			             return objectives.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (jumpObjectiveIdx == -1) {
			if (!objectives.empty()) {
				this->goToObjectiveUuid_ = objectives.back()->getUuid();
			}
		}
	}

	utils::ToggleButton(local.get("general.condition").c_str(), &this->condition_);

	ImGui::BeginDisabled(!this->condition_);

	ImGui::EndDisabled();
}

ktwait JumpToObjectiveObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &objectives = scene->getObjectives();

	const int jumpObjectiveIdx = utils::indexByUuid(objectives, this->goToObjectiveUuid_);

	if (jumpObjectiveIdx == -1) {
		setObjectiveError(result, *this, NotSelected, "The objective is not selected.");
		co_return;
	}

	if (this->condition_) { } else {
		ProjectPlayerService::getInstance().setNextObjective(jumpObjectiveIdx);
	}

	co_return;
}
