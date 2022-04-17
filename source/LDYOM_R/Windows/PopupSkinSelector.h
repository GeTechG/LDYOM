#pragma once
#include "functional"
#include "optional"
#include "../Data/Texture.h"

class PopupSkinSelector {
private:
	std::unordered_map<int, Texture> pedsIcons_;
	std::unordered_map<int, Texture> specialPedIcons_;
	std::optional<Texture> unknownIcon_;

	bool isOpen_ = false;

	bool filteringPed(int model) const;

	PopupSkinSelector () = default;
	PopupSkinSelector (const PopupSkinSelector & root) = delete;
	PopupSkinSelector & operator=(const PopupSkinSelector &) = delete;
public:
	static PopupSkinSelector & getInstance() {
		static PopupSkinSelector  instance;
		return instance;
	}

	void Init();

	void showPopup();
	void renderPopup(const std::function<void(int)>& onSelectCallback, bool special = false, int slot = 0);
};

