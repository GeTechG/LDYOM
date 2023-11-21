#include "WaitSignalObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "strUtils.h"

WaitSignalObjective::WaitSignalObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.wait_signal"));
	this->name += suffix;
}

void WaitSignalObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::Text(local.get("objective.nothing_params").c_str());
}

ktwait WaitSignalObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	while (!isEmitSignal_) {
		co_await 1;
	}
	isEmitSignal_ = false;
}

void WaitSignalObjective::emitSignal() {
	isEmitSignal_ = true;
}
