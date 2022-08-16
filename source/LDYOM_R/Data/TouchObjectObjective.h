#pragma once
#include "ObjectObjective.h"

class TouchObjectObjective final : virtual public ObjectObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar& boost::serialization::base_object<ObjectObjective>(*this);
	}

public:
	TouchObjectObjective() = default;
	explicit TouchObjectObjective(void* _new);
	~TouchObjectObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	ktwait execute(Scene * scene, Object * object, Result & result, ktcoro_tasklist & tasklist) override;
};
