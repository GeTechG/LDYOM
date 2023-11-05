#include "WeatherObjective.h"

#include <CWeather.h>

#include "strUtils.h"
#include "utils.h"

WeatherObjective::WeatherObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.weather"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void WeatherObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	if (utils::Combo(local.get("general.weather").c_str(), &this->weather_, local.getArray("general.weathers_types"))) {
		CWeather::ForceWeatherNow(static_cast<short>(this->weather_));
	}
}

ktwait WeatherObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	CWeather::ForceWeatherNow(static_cast<short>(this->weather_));
	co_return;
}
