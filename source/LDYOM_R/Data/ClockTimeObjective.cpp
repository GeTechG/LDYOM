#include "ClockTimeObjective.h"

#include <CClock.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

ClockTimeObjective::ClockTimeObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.clock_time"));
	this->name += suffix;
}

std::array<signed char, 2>& ClockTimeObjective::getTime() { return time_; }

void ClockTimeObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	bool timeChanged = false;
	constexpr auto hourMin = static_cast<ImS8>(0);
	constexpr auto hourMax = static_cast<ImS8>(23);
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5.f);
	timeChanged |= ImGui::SliderScalar(local.get("general.hours").c_str(), ImGuiDataType_S8, &this->time_[0], &hourMin,
	                                   &hourMax);
	ImGui::SameLine();
	constexpr auto minutesMin = static_cast<ImS8>(0);
	constexpr auto minutesMax = static_cast<ImS8>(59);
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5.f);
	timeChanged |= ImGui::SliderScalar(local.get("general.minutes").c_str(), ImGuiDataType_S8, &this->time_[1],
	                                   &minutesMin, &minutesMax);
	if (timeChanged) {
		CClock::SetGameClock(this->time_[0], this->time_[1], 0);
	}
}

ktwait ClockTimeObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	CClock::SetGameClock(this->time_[0], this->time_[1], 0);
	co_return;
}
