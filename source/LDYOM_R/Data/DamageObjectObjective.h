#pragma once
#include "ObjectObjective.h"

class DamageObjectObjective : virtual public ObjectObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ObjectObjective>(*this);
		ar & type_;
		ar & weaponId_;
	}

	int type_ = 0;
	int weaponId_ = 1;
public:
	DamageObjectObjective() = default;
	explicit DamageObjectObjective(void* _new);
	~DamageObjectObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	
	void draw(Localization& local) override;
	ktwait execute(Scene* scene, Object* object, Result& result, ktcoro_tasklist& tasklist) override;
};
