#pragma once
#include "WorldObjective.h"
#include "glm/ext/scalar_constants.hpp"

class DettachPedToEntity final : virtual public WorldObjective {
private:
	boost::uuids::uuid entityDettachUuid_{};
	int dettachType = 0;

public:
	DettachPedToEntity() = default;
	explicit DettachPedToEntity(void *ptr);
	~DettachPedToEntity() override = default;

	int getTypeCategory() override {
		return 12;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	boost::uuids::uuid& getEntityDettachUuid();
	int& getDettachType();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<DettachPedToEntity> {
		static void to_json(json &j, const DettachPedToEntity &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<DettachPedToEntity&>(obj);
			j["entityDettachUuid"] = a.getEntityDettachUuid();
			j["dettachType"] = a.getDettachType();
		}

		static void from_json(const json &j, DettachPedToEntity &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("entityDettachUuid").get_to(obj.getEntityDettachUuid());
			j.at("dettachType").get_to(obj.getDettachType());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
