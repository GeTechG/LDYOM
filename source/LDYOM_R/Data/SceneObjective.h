#pragma once
#include "BaseObjective.h"

class SceneObjective : virtual public BaseObjective {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<BaseObjective>(*this);
	}
public:
	int getCategory() override {
		return 7;
	}
};
