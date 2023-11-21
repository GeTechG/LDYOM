#pragma once
#include "PlayerObjective.h"

class RemoveWeaponsObjective final : public virtual PlayerObjective {
public:
	RemoveWeaponsObjective() = default;
	explicit RemoveWeaponsObjective(void *_new);
	~RemoveWeaponsObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<RemoveWeaponsObjective> {
		static void to_json(json &j, const RemoveWeaponsObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			// Additional serialization for RemoveWeaponsObjective if needed
		}

		static void from_json(const json &j, RemoveWeaponsObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::from_json(j, playerObjective);
			// Additional deserialization for RemoveWeaponsObjective if needed
		}
	};

NLOHMANN_JSON_NAMESPACE_END
