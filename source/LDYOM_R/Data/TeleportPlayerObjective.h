#pragma once
#include <CPed.h>

#include "PlayerObjective.h"

class CVector;

class TeleportPlayerObjective final : public virtual PlayerObjective {
private:
	std::array<float, 3> pos_{};
	float headingAngle_{};
	unsigned char modelType_{};
	int slot_{};
	int modelId_{};
	int health_ = 100;
	int interiorID_ = 0;
	bool dressUp_ = true;
	std::array<unsigned, 10> clotherMAnModelKeys_{};
	std::array<unsigned, 18> clotherMAnTextureKeys_{};
	float fatStat = 1.f;
	float musculeStat = 1.f;

	std::optional<CPed*> editorPed_;

	CPed* spawnPed();
	void characteristicsSection(Localization &local);

public:
	TeleportPlayerObjective() = default;
	explicit TeleportPlayerObjective(CVector &pos, float heading);
	~TeleportPlayerObjective() override;

	int getTypeCategory() override {
		return 0;
	}

	std::array<float, 3>& getPos();
	float& getHeadingAngle();
	unsigned char& getModelType();
	int& getSlot();
	int& getModelId();
	int& getHealth();
	int& getInteriorId();
	bool& isDressUp();
	std::array<unsigned, 10>& getClotherMAnModelKeys();
	std::array<unsigned, 18>& getClotherMAnTextureKeys();
	float& getFatStat();
	float& getMusculeStat();
	std::optional<CPed*>& getEditorPed();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	void spawnEditorPed();
	void deleteEditorPed();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<TeleportPlayerObjective> {
		static void to_json(json &j, const TeleportPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<TeleportPlayerObjective&>(obj);

			j["pos"] = a.getPos();
			j["headingAngle"] = a.getHeadingAngle();
			j["modelType"] = a.getModelType();
			j["slot"] = a.getSlot();
			j["modelId"] = a.getModelId();
			j["health"] = a.getHealth();
			j["interiorID"] = a.getInteriorId();
			j["dressUp"] = a.isDressUp();
			j["clotherMAnModelKeys"] = a.getClotherMAnModelKeys();
			j["clotherMAnTextureKeys"] = a.getClotherMAnTextureKeys();
			j["fatStat"] = a.getFatStat();
			j["musculeStat"] = a.getMusculeStat();
		}

		static void from_json(const json &j, TeleportPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::from_json(j, playerObjective);

			j.at("pos").get_to(obj.getPos());
			j.at("headingAngle").get_to(obj.getHeadingAngle());
			j.at("modelType").get_to(obj.getModelType());
			j.at("slot").get_to(obj.getSlot());
			j.at("modelId").get_to(obj.getModelId());
			j.at("health").get_to(obj.getHealth());
			j.at("interiorID").get_to(obj.getInteriorId());
			j.at("dressUp").get_to(obj.isDressUp());
			j.at("clotherMAnModelKeys").get_to(obj.getClotherMAnModelKeys());
			j.at("clotherMAnTextureKeys").get_to(obj.getClotherMAnTextureKeys());
			j.at("fatStat").get_to(obj.getFatStat());
			j.at("musculeStat").get_to(obj.getMusculeStat());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
