#pragma once
#include "WorldObjective.h"

class ClockTimeObjective final : virtual public WorldObjective {
private:
	std::array<signed char, 2> time_ = {8, 0};

public:
	ClockTimeObjective() = default;
	explicit ClockTimeObjective(void *_new);
	~ClockTimeObjective() override = default;

	int getTypeCategory() override {
		return 5;
	}

	std::array<signed char, 2>& getTime();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<ClockTimeObjective> {
		static void to_json(json &j, const ClockTimeObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<ClockTimeObjective&>(obj);
			j["time"] = a.getTime();
		}

		static void from_json(const json &j, ClockTimeObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("time").get_to(obj.getTime());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
