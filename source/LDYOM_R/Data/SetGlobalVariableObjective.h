#pragma once
#include "WorldObjective.h"

class SetGlobalVariableObjective: virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & varId_;
		ar & valueNumber_;
		ar & valueBoolean_;
	}

	int varId_ = 1;
	float valueNumber_ = 0.f;
	bool valueBoolean_ = false;
public:
	SetGlobalVariableObjective() = default;
	explicit SetGlobalVariableObjective(void* _new);
	~SetGlobalVariableObjective() override = default;

	int getTypeCategory() override {
		return 9;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
