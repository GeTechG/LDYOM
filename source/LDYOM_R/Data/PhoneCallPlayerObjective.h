#pragma once
#include "PlayerObjective.h"


struct Dialog {
	std::string text;
	float textTime = 2.f;
	bool moveMouth = true;
	std::string gameText;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Dialog> {
		static void to_json(json &j, const Dialog &obj) {
			j["text"] = obj.text;
			j["moveMouth"] = obj.moveMouth;
			j["textTime"] = obj.textTime;
		}

		static void from_json(const json &j, Dialog &obj) {
			j.at("text").get_to(obj.text);
			j.at("moveMouth").get_to(obj.moveMouth);
			j.at("textTime").get_to(obj.textTime);
		}
	};

NLOHMANN_JSON_NAMESPACE_END

class PhoneCallPlayerObjective final : public virtual PlayerObjective {
	std::vector<Dialog> dialogs_;

public:
	PhoneCallPlayerObjective() = default;
	explicit PhoneCallPlayerObjective(void *_new);
	~PhoneCallPlayerObjective() override = default;

	int getTypeCategory() override {
		return 6;
	}

	std::vector<Dialog>& getDialogs();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<PhoneCallPlayerObjective> {
		static void to_json(json &j, const PhoneCallPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<PhoneCallPlayerObjective&>(obj);
			j["dialogs"] = a.getDialogs(); // Assuming getDialogs() returns the dialogs_ member.
		}

		static void from_json(const json &j, PhoneCallPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			j.get_to(playerObjective);
			j.at("dialogs").get_to(obj.getDialogs()); // Assuming getDialogs() exists to set the dialogs_ member.
		}
	};

NLOHMANN_JSON_NAMESPACE_END
