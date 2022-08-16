#pragma once
#include "BaseObjective.h"

class PlayerObjective: virtual public BaseObjective {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<BaseObjective>(*this);
	}
protected:
	PlayerObjective() = default;
public:
	int getCategory() override {
		return 5;
	}
};
