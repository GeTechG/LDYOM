#pragma once
#include "SceneObjective.h"

class WaitSignalObjective final: public virtual SceneObjective {
private:
	inline static bool isEmitSignal_ = false;

	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<SceneObjective>(*this);
	}

public:
	WaitSignalObjective() = default;
	explicit WaitSignalObjective(void* _new);
	~WaitSignalObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
	static void emitSignal();
};
