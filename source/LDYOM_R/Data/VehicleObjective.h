#pragma once
#include <CVehicle.h>

#include "BaseObjective.h"

class Vehicle;

class VehicleObjective : virtual public BaseObjective {
protected:
	boost::uuids::uuid vehicleUuid_{};
	std::string text;
	float textTime_ = 1.f;
	int colorBlip_ = 0;

	std::string gameText = {""};

	VehicleObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(CVehicle *vehicle);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	virtual ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) = 0;

public:
	~VehicleObjective() override;

	int getCategory() override {
		return 2;
	}

	boost::uuids::uuid& getVehicleUuid();
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
	struct adl_serializer<VehicleObjective> {
		static void to_json(json &j, const VehicleObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);

			auto &a = const_cast<VehicleObjective&>(obj);
			j["vehicleUuid"] = a.getVehicleUuid();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["colorBlip"] = a.getColorBlip();
		}

		static void from_json(const json &j, VehicleObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			j.get_to(baseObjective);

			j.at("vehicleUuid").get_to(obj.getVehicleUuid());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("colorBlip").get_to(obj.getColorBlip());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
