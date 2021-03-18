#pragma once

#include "ImGUI/imgui.h"
#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")
#include "plugin.h"
#include <vector>
#include <map>
#include <CTheScripts.h>
#include "extensions/ScriptCommands.h"
#include <CModelInfo.h>
#include <CText.h>
#include "Init.h"

using namespace plugin;
using namespace std;

struct VehicleInfo {
	char name[33];
	int id;
	PDIRECT3DTEXTURE9 pic;
};

class carSelector {
public:
	static bool bInitVehicles;
	static vector<VehicleInfo> vehicles;
	static int typeSearch;
	static char search[65];
	static bool filter_car;
	static bool filter_bike;
	static bool filter_mtrack;
	static bool filter_quad;
	static bool filter_plane;
	static bool filter_heli;
	static bool filter_bmx;
	static bool filter_trailer;
	static bool filter_boat;
	static bool bShow;
	static float scale;
	static int* modelID;

	static void fShow();
	static void fInitVehicles();
};

