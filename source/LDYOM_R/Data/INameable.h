#pragma once
#include <string>

class INameable {
public:
	virtual ~INameable() = default;

	virtual std::string& getName() = 0;
};
