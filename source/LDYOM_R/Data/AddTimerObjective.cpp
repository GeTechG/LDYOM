﻿#include "AddTimerObjective.h"

#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "strUtils.h"
#include "TimerService.h"
#include "utils.h"

AddTimerObjective::AddTimerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.add_timer"));
	this->name += suffix;
}

void AddTimerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	utils::Combo(local.get("general.type").c_str(), &this->typeTimer_, local.getArray("timer.types"));
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(local.get("timer.help_timer_types").c_str());

	ImGui::InputInt(local.get("timer.start_time").c_str(), &this->startTime_);
	utils::Combo(local.get("timer.condition_end").c_str(), reinterpret_cast<int*>(&this->compareType_),
	             local.getArray("general.compare_types"));
	ImGui::InputInt("##conditionValue", &this->compareValue_);
	utils::ToggleButton(local.get("timer.backwards").c_str(), &this->backward_);
	ImGui::InputText(local.get("general.text").c_str(), &this->text);
}

ktwait AddTimerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	if (TimerService::getInstance().isTimerOn()) {
		result.setError(2, "Another timer is running at the moment.");
		co_return;
	}

	auto cp1251Text = utf8ToCp1251(this->text);
	gxtEncode(cp1251Text);
	this->gameText = cp1251Text;

	Command<0x0ADF>("LDTIMER0", this->gameText.data());

	TimerService::getInstance().addTimer("LDTIMER0", this->backward_, this->startTime_ * 1000);

	auto process = [](AddTimerObjective *this_) -> ktwait {
		std::function action = [] {
			TimerService::getInstance().removeTimer();
		};
		if (this_->getTypeTimer() == 1)
			action = [] {
				//failed action
				TimerService::getInstance().removeTimer();
			};
		while (TimerService::getInstance().isTimerOn()) {
			switch (this_->getCompareType()) {
				case Equal:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 == this_->getCompareValue() * 1000)
						action();
					break;
				case NotEqual:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 != this_->getCompareValue() * 1000)
						action();
					break;
				case More:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 > this_->getCompareValue() * 1000)
						action();
					break;
				case MoreOrEqual:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 >= this_->getCompareValue() * 1000)
						action();
					break;
				case Less:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 < this_->getCompareValue() * 1000)
						action();
					break;
				case LessOrEqual:
					if (TimerService::getInstance().getTimerTime() / 1000 * 1000 <= this_->getCompareValue() * 1000)
						action();
					break;
			}

			co_await 1;
		}
	};

	if (this->typeTimer_ == 0) {
		co_await process(this);
	} else {
		tasklist.add_task(process, this);
	}
}

int& AddTimerObjective::getTypeTimer() {
	return typeTimer_;
}

MathCondition& AddTimerObjective::getCompareType() {
	return compareType_;
}

int& AddTimerObjective::getCompareValue() {
	return compareValue_;
}

bool& AddTimerObjective::isBackward() { return backward_; }
int& AddTimerObjective::getStartTime() { return startTime_; }
std::string& AddTimerObjective::getText() { return text; }
