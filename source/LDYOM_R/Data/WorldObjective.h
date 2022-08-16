#pragma once
#include <boost/serialization/base_object.hpp>

#include "BaseObjective.h"

class WorldObjective: virtual public BaseObjective {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseObjective>(*this);
	}
protected:
	WorldObjective() = default;
	WorldObjective(void* _new);
public:
	int getCategory() override {
		return 0;
	}
};
