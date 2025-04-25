#pragma once
#include "WorldObjective.h"

class CountdownObjective final : virtual public WorldObjective {
private:
	int time = 3;
	std::string textGo{"Go!"};

	std::string gameTextGo;

public:
	CountdownObjective() = default;
	explicit CountdownObjective(void *_new);
	~CountdownObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	int& getTime();
	std::string& getTextGo();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CountdownObjective> {
		static void to_json(json &j, const CountdownObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<CountdownObjective&>(obj);
			j["time"] = a.getTime();
			j["textGo"] = a.getTextGo();
		}

		static void from_json(const json &j, CountdownObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("time").get_to(obj.getTime());
			j.at("textGo").get_to(obj.getTextGo());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
