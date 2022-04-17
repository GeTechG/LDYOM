#pragma once
#include "functional"
#include "optional"
#include "../Data/Texture.h"

class PopupWeaponSelector {
private:
	std::unordered_map<int, Texture> icons_;
	std::optional<Texture> unknownIcon_;

	PopupWeaponSelector() = default;
	PopupWeaponSelector(const PopupWeaponSelector & root) = delete;
	PopupWeaponSelector& operator=(const PopupWeaponSelector &) = delete;
public:
	static PopupWeaponSelector& getInstance() {
		static PopupWeaponSelector instance;
		return instance;
	}

	void Init();

	void showPopup() const;
	void weaponButton(const int* value);
	void renderPopup(const std::function<void(int)>& onSelectCallback);
};

