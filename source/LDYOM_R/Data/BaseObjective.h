#pragma once
#include <array>
#include <ktcoro_wait.hpp>
#include <boost/serialization/access.hpp>

#include "constants.h"
#include "INameable.h"
#include "IUuidable.h"
#include "Localization/Localization.h"
#include "boost/uuid/uuid_serialize.hpp"
#include <boost/serialization/array.hpp>
#include "fmt/core.h"

#include "Result.h"

class Result;
class Scene;

class BaseObjective: public INameable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & this->uuid_;
		ar & boost::serialization::make_array(this->name_.data(), this->name_.size());
	}
protected:
	BaseObjective();
	BaseObjective(void* _new);
	BaseObjective(const BaseObjective& other);

	boost::uuids::uuid uuid_;

	std::array<char, NAME_SIZE> name_;
public:
	~BaseObjective() override = default;

	boost::uuids::uuid& getUuid() override;

	char* getName() override;

	virtual int getCategory() = 0;
	virtual int getTypeCategory() = 0;
	virtual void draw(Localization& local) = 0;
	virtual ktwait execute(Scene* scene, Result& result) = 0;
};

inline void setObjectiveError(Result& result, BaseObjective& objective, unsigned code, std::string message) {
	result.setError(code, fmt::format("{}\nObjective: {}", message, objective.getName()));
}
