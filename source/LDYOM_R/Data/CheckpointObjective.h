#pragma once

#include "WorldObjective.h"
#include "boost/serialization/array.hpp"
#include "Localization/Localization.h"

#include "../Windows/PopupSpriteBlipSelector.h"

class CheckpointObjective final : virtual public WorldObjective {
private:
	boost::uuids::uuid checkpointUuid_;
	std::string text_;
	int onWhatArrive_;
	float textTime_;
	boost::uuids::uuid comeBackVehicle_;
	std::string textComeBackVehicle_;
	int colorBlipComeBackVehicle_;
	std::optional<int> projectComeBackBlip_;

	std::string gameText_ = {""};
	PopupSpriteBlipSelector popupSpriteBlipSelector_;

public:
	CheckpointObjective() = default;
	explicit CheckpointObjective(void *_new);

	CheckpointObjective(const CheckpointObjective &other);
	CheckpointObjective& operator=(const CheckpointObjective &other);
	~CheckpointObjective() override;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
	void draw(Localization &local, std::vector<std::string> &listOverlay) override;

	boost::uuids::uuid& getCheckpointUuid();
	std::string& getText();
	int& getOnWhatArrive();
	float& getTextTime();
	boost::uuids::uuid& getComeBackVehicle();
	std::string& getTextComeBackVehicle();
	int& getColorBlipComeBackVehicle();
	std::optional<int>& getProjectComeBackBlip();
	std::string& getGameText();

	void removeProjectComeBackBlip();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CheckpointObjective> {
		static void to_json(json &j, const CheckpointObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<CheckpointObjective&>(obj);
			j["checkpointUuid"] = a.getCheckpointUuid();
			j["text"] = a.getText();
			j["onWhatArrive"] = a.getOnWhatArrive();
			j["textTime"] = a.getTextTime();
			j["comeBackVehicle"] = a.getComeBackVehicle();
			j["textComeBackVehicle"] = a.getTextComeBackVehicle();
			j["colorBlipComeBackVehicle"] = a.getColorBlipComeBackVehicle();
		}

		static void from_json(const json &j, CheckpointObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("checkpointUuid").get_to(obj.getCheckpointUuid());
			j.at("text").get_to(obj.getText());
			j.at("onWhatArrive").get_to(obj.getOnWhatArrive());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("comeBackVehicle").get_to(obj.getComeBackVehicle());
			j.at("textComeBackVehicle").get_to(obj.getTextComeBackVehicle());
			j.at("colorBlipComeBackVehicle").get_to(obj.getColorBlipComeBackVehicle());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
