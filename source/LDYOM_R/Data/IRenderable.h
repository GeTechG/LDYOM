#pragma once

class IRenderable {
public:
	virtual ~IRenderable() = default;

	virtual void render() = 0;
};
