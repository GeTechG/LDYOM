#pragma once
#include "WorldObjective.h"

class RemoveTimerObjective final : public virtual WorldObjective {
public:
	RemoveTimerObjective() = default;
	explicit RemoveTimerObjective(void *_new);
	~RemoveTimerObjective() override = default;

	int getTypeCategory() override {
		return 8;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<RemoveTimerObjective> {
		static void to_json(json &j, const RemoveTimerObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<RemoveTimerObjective&>(obj);
		}

		static void from_json(const json &j, RemoveTimerObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
