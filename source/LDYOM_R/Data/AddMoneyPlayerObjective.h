#pragma once
#include "PlayerObjective.h"

class AddMoneyPlayerObjective final : public virtual PlayerObjective {
	int money_ = 0;

public:
	AddMoneyPlayerObjective() = default;
	explicit AddMoneyPlayerObjective(void *_new);
	~AddMoneyPlayerObjective() override = default;

	int getTypeCategory() override {
		return 7;
	}

	int& getMoney();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AddMoneyPlayerObjective> {
		static void to_json(json &j, const AddMoneyPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<AddMoneyPlayerObjective&>(obj);
			j["money"] = a.getMoney();
		}

		static void from_json(const json &j, AddMoneyPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			j.get_to(playerObjective);
			j.at("money").get_to(obj.getMoney());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
