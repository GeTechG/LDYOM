#include "RemoveTimerObjective.h"

#include "imgui.h"
#include "strUtils.h"
#include "TimerService.h"

RemoveTimerObjective::RemoveTimerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.remove_timer"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void RemoveTimerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::Text(local.get("objective.nothing_params").c_str());
}

ktwait RemoveTimerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	TimerService::getInstance().removeTimer();
	co_return;
}
