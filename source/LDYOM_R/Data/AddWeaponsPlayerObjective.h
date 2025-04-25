#pragma once
#include "PlayerObjective.h"
#include "Weapon.h"

class AddWeaponsPlayerObjective final : public virtual PlayerObjective {
private:
	std::vector<Weapon> weapons_;
	int defaultWeapon_{};

public:
	AddWeaponsPlayerObjective() = default;
	explicit AddWeaponsPlayerObjective(void *_new);
	~AddWeaponsPlayerObjective() override = default;

	int getTypeCategory() override {
		return 5;
	}

	std::vector<Weapon>& getWeapons();
	int& getDefaultWeapon();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AddWeaponsPlayerObjective> {
		static void to_json(json &j, const AddWeaponsPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<AddWeaponsPlayerObjective&>(obj);
			j["weapons"] = a.getWeapons();
			j["defaultWeapon"] = a.getDefaultWeapon();
		}

		static void from_json(const json &j, AddWeaponsPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			j.get_to(playerObjective);
			j.at("weapons").get_to(obj.getWeapons());
			j.at("defaultWeapon").get_to(obj.getDefaultWeapon());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
