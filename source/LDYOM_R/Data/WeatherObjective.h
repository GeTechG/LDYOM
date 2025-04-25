#pragma once
#include "WorldObjective.h"

class WeatherObjective final : virtual public WorldObjective {
private:
	int weather_ = 0;

public:
	WeatherObjective() = default;
	explicit WeatherObjective(void *_new);
	~WeatherObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	int& getWeather();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<WeatherObjective> {
		static void to_json(json &j, const WeatherObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<WeatherObjective&>(obj);
			j["weather"] = a.getWeather();
		}

		static void from_json(const json &j, WeatherObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("weather").get_to(obj.getWeather());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
