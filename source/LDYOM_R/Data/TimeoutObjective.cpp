#include "TimeoutObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "strUtils.h"

TimeoutObjective::TimeoutObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.timeout"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void TimeoutObjective::draw(Localization &local) {
	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::InputFloat(local.get("objective.timeout").c_str(), &this->time_);
}

ktwait TimeoutObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->time_ * 1000.f), 0, false);
	co_await std::chrono::milliseconds(static_cast<long long>(this->time_ * 1000.f));
}
