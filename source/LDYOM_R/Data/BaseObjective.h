#pragma once
#include <ktcoro_wait.hpp>
#include <boost/serialization/access.hpp>

#include "constants.h"
#include "INameable.h"
#include "IUuidable.h"
#include "Localization/Localization.h"
#include "boost/uuid/uuid_serialize.hpp"

class Scene;

class BaseObjective: public INameable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & this->uuid_;
		ar & this->name_;
	}
protected:
	BaseObjective();
	BaseObjective(void* _new);
	BaseObjective(const BaseObjective& other);

	boost::uuids::uuid uuid_;

	char name_[NAME_SIZE]{};
public:
	~BaseObjective() = default;

	boost::uuids::uuid& getUuid() override;

	char* getName() override;

	virtual int getCategory() = 0;
	virtual int getTypeCategory() = 0;
	virtual void draw(Localization& local) = 0;
	virtual ktwait execute(Scene* scene) = 0;
};
