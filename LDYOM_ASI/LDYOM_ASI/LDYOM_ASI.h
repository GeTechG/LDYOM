#pragma once

#include "plugin.h"
#include "iostream"
#include <fstream>
#include "common.h"
#include <string>
#include "libs/memsafe.h"
#include <CMessages.h>
#include <CMenuManager.h>
#include <CHud.h>
#include <CCheat.h>
#include <CWorld.h>
#include <CGame.h>
#include <CWeather.h>
#include <CCamera.h>
#include <CTheScripts.h>
#include "extensions/ScriptCommands.h"
#include <vector>
#include <string>
#include <typeinfo>
#include "resource.h"
#include <thread>
#include "CStreaming.h"
#include <CClothes.h>
#include <CClock.h>
#include <CCivilianPed.h>
#include <functional>
#include "CModelInfo.h"
#include "CMonsterTruck.h"
#include "CQuadBike.h"
#include "CHeli.h"
#include "CPlane.h"
#include "CBmx.h"
#include "CTrailer.h"
#include "CBoat.h"
#include "libs/additional_events.h"
#include "CTaskComplexEnterCarAsDriver.h"
#include "CObject.h"
#include "CExplosion.h"
#include "CPickup.h"

// Link to lua library
#ifdef _WIN32
#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "luajit.lib")
#pragma comment(lib, "buildvm.lib")
#endif
#include <lua.hpp>
#include <sol/sol.hpp>


#include "libs/coro_wait.h"

class NodeGraph;
using namespace std::chrono_literals;

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx9.h"
#include "ImGUI/imgui_impl_win32.h"
#include "libs/imguistyleloader.h"
#include "VMTHooker/vmt.h"
#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>

using namespace plugin;
using namespace memsafe;
using namespace std;

const char VERSION_STR[11] = "Beta 0.7.2";
const std::string authors[2] = { "SKIC","SIZZZ" };
bool init = false;
CPed *playerPed;
extern char *Language;

bool runningThreads = true;

#include "libs/ini.h"

mINI::INIStructure lang_file;
std::string curr_lang_string;
std::string curr_theme_string;
vector <std::string> names_langs;
vector <std::string> names_themes;
map <std::string, std::vector<std::string>> langMenu;
bool updateSphere = false;
std::string* nameCurrPack = nullptr;
int fastdata_pack = -1;
int fastdata_miss = -1;
HMODULE hhModule;
extern bool mission_started;
int curr_theme;
int curr_lang;


#include "libs/MyFuncs.h"
#include "Init.h"
Mission* currentMissionPtr;
NodeGraph* currentNodeGraphPtr;
Storyline* currentStorylinePtr;
map<std::string, std::vector<sol::table>> map_nodes_class;

void beginThread(void* __startAddress) { 
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)__startAddress, NULL, NULL, NULL);
}

void createDirsLDYOM();

LONG prevWndProc;
coro_wait instance;

#include "carSelector.h"
#include "render.h"