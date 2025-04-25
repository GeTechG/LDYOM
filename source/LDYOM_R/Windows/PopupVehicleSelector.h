#pragma once
#include <optional>

#include "plugin.h"
#include <vector>

#include "AbstractWindow.h"
#include "ModelRenderer.h"
#include "../Data/Texture.h"

struct ImVec2;
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
	std::unordered_map<int, RwTexture*> unknownVehicles_;
	std::unique_ptr<Texture> unknownIcon_;

	static optional<ModelRenderer> renderer_;

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

	void clearUnknownVehicles();
	std::pair<IDirect3DTexture9*, ImVec2> getModelIcon(int modelId);
public:
	PopupVehicleSelector();

	void show();

	void draw(std::function<void(int model)> onSelect);
};

