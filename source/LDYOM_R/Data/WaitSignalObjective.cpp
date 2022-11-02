#include "WaitSignalObjective.h"

#include "imgui.h"
#include "strUtils.h"

WaitSignalObjective::WaitSignalObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.wait_signal"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void WaitSignalObjective::draw(Localization& local) {
	ImGui::Dummy(ImVec2(400.f, 0.f));
}

ktwait WaitSignalObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	while (!isEmitSignal_) {
		co_await 1;
	}
	isEmitSignal_ = false;
}

void WaitSignalObjective::emitSignal() {
	isEmitSignal_ = true;
}
