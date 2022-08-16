#pragma once
#include "ObjectObjective.h"

class PhotographObjectObjective final : virtual public ObjectObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ObjectObjective>(*this);
	}

public:
	PhotographObjectObjective() = default;
	explicit PhotographObjectObjective(void* _new);
	~PhotographObjectObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene * scene, Object * object, Result & result, ktcoro_tasklist & tasklist) override;
};
