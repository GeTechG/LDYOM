#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "../Data/Texture.h"

class PopupSpriteBlipSelector {
private:
	inline static std::unordered_map<int, std::unique_ptr<Texture>> blipIcons_ = {};

	bool open_ = false;
public:
	static void Init();

	void draw(int* blip, std::function<void(int)> callback);
};
