#pragma once
#include <CPed.h>
#include <nlohmann/json.hpp>

#include "BaseObjective.h"

class Actor;

class ActorObjective : virtual public BaseObjective {
protected:
	boost::uuids::uuid actorUuid{};
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;
	ActorObjective() = default;

	std::optional<int> editorBlip;
	std::optional<int> projectBlip;

	int spawnBlip(CPed *ped);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	virtual ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) = 0;

public:
	ActorObjective(const ActorObjective &other)
		: BaseObjective{other},
		  actorUuid{other.actorUuid},
		  text{other.text},
		  textTime{other.textTime},
		  colorBlip{other.colorBlip},
		  gameText{other.gameText} {}

	~ActorObjective() override;

	int getCategory() override {
		return 1;
	}

	boost::uuids::uuid& getActorUuid();
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
	struct adl_serializer<ActorObjective> {
		static void to_json(json &j, const ActorObjective &obj) {
			auto &base = static_cast<const BaseObjective&>(obj);
			j = base;
			auto &a = const_cast<ActorObjective&>(obj);
			j["actorUuid"] = a.getActorUuid();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["colorBlip"] = a.getColorBlip();
		}

		static void from_json(const json &j, ActorObjective &obj) {
			auto &base = static_cast<BaseObjective&>(obj);
			j.get_to(base);
			j.at("actorUuid").get_to(obj.getActorUuid());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("colorBlip").get_to(obj.getColorBlip());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
