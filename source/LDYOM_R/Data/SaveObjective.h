#pragma once
#include <functional>

#include "SceneObjective.h"

class SaveObjective final : public virtual SceneObjective {
private:
	bool confirmSave_ = false;

public:
	SaveObjective() = default;
	explicit SaveObjective(void *_new);
	~SaveObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<SaveObjective> {
		static void to_json(json &j, const SaveObjective &obj) {
			auto &sceneObjective = static_cast<const SceneObjective&>(obj);
			adl_serializer<SceneObjective>::to_json(j, sceneObjective);
			auto &a = const_cast<SaveObjective&>(obj);
		}

		static void from_json(const json &j, SaveObjective &obj) {
			auto &sceneObjective = static_cast<SceneObjective&>(obj);
			j.get_to(sceneObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
