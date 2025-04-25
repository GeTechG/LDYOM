#pragma once
#include "ObjectObjective.h"

class DamageObjectObjective : virtual public ObjectObjective {
	int type_ = 0;
	int weaponId_ = 1;

public:
	DamageObjectObjective() = default;
	explicit DamageObjectObjective(void *_new);
	~DamageObjectObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	int& getType();
	int& getWeaponId();


	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<DamageObjectObjective> {
		static void to_json(json &j, const DamageObjectObjective &obj) {
			auto &objectObjective = static_cast<const ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::to_json(j, objectObjective);
			auto &a = const_cast<DamageObjectObjective&>(obj);
			j["type"] = a.getType();
			j["weaponId"] = a.getWeaponId();
		}

		static void from_json(const json &j, DamageObjectObjective &obj) {
			auto &objectObjective = static_cast<ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::from_json(j, objectObjective);

			j.at("type").get_to(obj.getType());
			j.at("weaponId").get_to(obj.getWeaponId());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
