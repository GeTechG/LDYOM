#pragma once
#include "SceneObjective.h"

class WaitSignalObjective final : public virtual SceneObjective {
private:
	inline static bool isEmitSignal_ = false;

public:
	WaitSignalObjective() = default;
	explicit WaitSignalObjective(void *_new);
	~WaitSignalObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
	static void emitSignal();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<WaitSignalObjective> {
		static void to_json(json &j, const WaitSignalObjective &obj) {
			auto &sceneObjective = static_cast<const SceneObjective&>(obj);
			adl_serializer<SceneObjective>::to_json(j, sceneObjective);
			// Add additional serialization for WaitSignalObjective if needed
		}

		static void from_json(const json &j, WaitSignalObjective &obj) {
			auto &sceneObjective = static_cast<SceneObjective&>(obj);
			j.get_to(sceneObjective);
			// Add additional deserialization for WaitSignalObjective if needed
		}
	};

NLOHMANN_JSON_NAMESPACE_END
