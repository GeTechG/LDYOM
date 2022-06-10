#pragma once
#include "plugin.h"
#include <vector>

#include "AbstractWindow.h"
#include "../Data/Texture.h"

using namespace plugin;
using namespace std;

struct VehicleInfo {
	char name[33];
	int id;
	PDIRECT3DTEXTURE9 pic;
};

class PopupVehicleSelector {
private:
	std::unordered_map<int, std::unique_ptr<Texture>> vehicleIcons_;
	std::unique_ptr<Texture> unknownIcon_;

	bool bInitVehicles = false;
	int typeSearch = 0;
	char search[65] = "";
	bool filter_car = true;
	bool filter_bike = true;
	bool filter_mtrack = true;
	bool filter_quad = true;
	bool filter_plane = true;
	bool filter_heli = true;
	bool filter_bmx = true;
	bool filter_trailer = true;
	bool filter_boat = true;
	bool bShow = true;
	float scale = 1.0f;
public:
	PopupVehicleSelector();

	void show();

	void draw(std::function<void(int model)> onSelect);
};

