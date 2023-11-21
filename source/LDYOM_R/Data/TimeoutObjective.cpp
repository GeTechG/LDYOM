#include "TimeoutObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "strUtils.h"

TimeoutObjective::TimeoutObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.timeout"));
	this->name += suffix;
}

float& TimeoutObjective::getTime() { return time_; }

std::string& TimeoutObjective::getText() { return text; }

void TimeoutObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::InputText(local.get("general.text").c_str(), &this->text);
	ImGui::InputFloat(local.get("objective.timeout").c_str(), &this->time_);
}

ktwait TimeoutObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	auto cp1251Text = utf8ToCp1251(this->text);
	gxtEncode(cp1251Text);
	this->gameText = cp1251Text;

	CMessages::AddMessage(const_cast<char*>(this->gameText.c_str()), static_cast<unsigned>(this->time_ * 1000.f), 0,
	                      false);
	co_await std::chrono::milliseconds(static_cast<long long>(this->time_ * 1000.f));
}
