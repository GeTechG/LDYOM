#pragma once

class INameable {
public:
	virtual ~INameable() = default;

	virtual char* getName() = 0;
};
