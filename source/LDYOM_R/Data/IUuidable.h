#pragma once

#include <boost/uuid/uuid.hpp>

class IUuidable {
public:
	virtual ~IUuidable() = default;

	virtual boost::uuids::uuid& getUuid() = 0;
};
