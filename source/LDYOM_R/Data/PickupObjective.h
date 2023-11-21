#pragma once

#include <CPickup.h>

#include "BaseObjective.h"

class Pickup;

class PickupObjective : virtual public BaseObjective {
	boost::uuids::uuid pickupUuid{};
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;

protected:
	PickupObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(int pickup);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	virtual ktwait execute(Scene *scene, Pickup *pickup, Result &result, ktcoro_tasklist &tasklist) = 0;

public:
	~PickupObjective() override;

	int getCategory() override {
		return 4;
	}

	boost::uuids::uuid& getPickupUuid();
	std::string& getText();
	float& getTextTime();
	int& getColorBlip();
	std::optional<int>& getEditorBlip();
	std::optional<int>& getProjectBlip();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	void open() override;
	void close() override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<PickupObjective> {
		static void to_json(json &j, const PickupObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
			auto &a = const_cast<PickupObjective&>(obj);
			j["pickupUuid"] = a.getPickupUuid();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["colorBlip"] = a.getColorBlip();
		}

		static void from_json(const json &j, PickupObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			adl_serializer<BaseObjective>::from_json(j, baseObjective);

			j.at("pickupUuid").get_to(obj.getPickupUuid());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("colorBlip").get_to(obj.getColorBlip());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
