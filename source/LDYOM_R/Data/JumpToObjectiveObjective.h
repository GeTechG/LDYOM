#pragma once
#include "SceneObjective.h"

class JumpToObjectiveObjective final : public virtual SceneObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<SceneObjective>(*this);
		ar & goToObjectiveUuid_;
		ar & condition_;
		ar & conditionType_;
		ar & varId_;
		ar & conditionValueNumber_;
		ar & conditionValueBoolean_;
	}

	boost::uuids::uuid goToObjectiveUuid_{};
	bool condition_ = true;
	int conditionType_ = 0;
	int varId_ = 1;
	float conditionValueNumber_ = 0.f;
	bool conditionValueBoolean_ = false;
public:
	JumpToObjectiveObjective() = default;
	explicit JumpToObjectiveObjective(void* _new);
	~JumpToObjectiveObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
