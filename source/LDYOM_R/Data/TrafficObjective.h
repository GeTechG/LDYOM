#pragma once
#include "WorldObjective.h"

class TrafficObjective final : virtual public WorldObjective {
private:
	float trafficPed_ = 1.f;
	float trafficCar_ = 1.f;

public:
	TrafficObjective() = default;
	explicit TrafficObjective(void *_new);
	~TrafficObjective() override = default;

	int getTypeCategory() override {
		return 6;
	}

	float& getTrafficPed();
	float& getTrafficCar();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<TrafficObjective> {
		static void to_json(json &j, const TrafficObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<TrafficObjective&>(obj);
			j["trafficPed"] = a.getTrafficPed();
			j["trafficCar"] = a.getTrafficCar();
		}

		static void from_json(const json &j, TrafficObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("trafficPed").get_to(obj.getTrafficPed());
			j.at("trafficCar").get_to(obj.getTrafficCar());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
