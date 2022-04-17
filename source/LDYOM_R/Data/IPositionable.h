#pragma once

class IPositionable {
public:
	virtual ~IPositionable() = default;

	virtual float* getPosition() = 0;
};
