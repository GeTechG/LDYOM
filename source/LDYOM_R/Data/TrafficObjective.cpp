#include "TrafficObjective.h"

#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "strUtils.h"

TrafficObjective::TrafficObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.traffic"));
	this->name += suffix;
}

float& TrafficObjective::getTrafficPed() { return trafficPed_; }

float& TrafficObjective::getTrafficCar() { return trafficCar_; }

void TrafficObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::DragFloat(local.get("general.ped_traffic").c_str(), &this->trafficPed_, 0.01f, 0.f, 2.f);
	ImGui::DragFloat(local.get("general.car_traffic").c_str(), &this->trafficCar_, 0.01f, 0.f, 2.f);
}

ktwait TrafficObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(this->trafficPed_);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(this->trafficCar_);
	co_return;
}
