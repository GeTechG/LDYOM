#pragma once
#include <CObject.h>

#include "BaseObjective.h"

class Object;

class ObjectObjective : virtual public BaseObjective {
private:
	boost::uuids::uuid objectUuid_{};
	std::string text{""};
	float textTime_ = 1.f;
	int colorBlip_ = 0;

	std::string gameText = {""};

protected:
	ObjectObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(CObject *object);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	virtual ktwait execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) = 0;

public:
	~ObjectObjective() override;

	int getCategory() override {
		return 3;
	}

	boost::uuids::uuid& getObjectUuid();
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
	struct adl_serializer<ObjectObjective> {
		static void to_json(json &j, const ObjectObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
			auto &a = const_cast<ObjectObjective&>(obj);
			j["objectUuid"] = a.getObjectUuid();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["colorBlip"] = a.getColorBlip();
		}

		static void from_json(const json &j, ObjectObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			adl_serializer<BaseObjective>::from_json(j, baseObjective);

			j.at("objectUuid").get_to(obj.getObjectUuid());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("colorBlip").get_to(obj.getColorBlip());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
