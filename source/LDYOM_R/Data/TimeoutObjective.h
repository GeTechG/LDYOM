#pragma once
#include "WorldObjective.h"

class TimeoutObjective final : virtual public WorldObjective {
private:
	float time_ = 1.f;
	std::string text;

	std::string gameText;

public:
	TimeoutObjective() = default;
	explicit TimeoutObjective(void *_new);
	~TimeoutObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	float& getTime();
	std::string& getText();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<TimeoutObjective> {
		static void to_json(json &j, const TimeoutObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<TimeoutObjective&>(obj);
			j["time"] = a.getTime();
			j["text"] = a.getText();
		}

		static void from_json(const json &j, TimeoutObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("time").get_to(obj.getTime());
			j.at("text").get_to(obj.getText());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
