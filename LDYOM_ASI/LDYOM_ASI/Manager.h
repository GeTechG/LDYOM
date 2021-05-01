#pragma once
#include "Init.h"
#include "NodeGraph.h"


class Manager
{
public:
	static void SaveMission(int curr_pack, int curr_miss);
	static void SaveListMission(int curr_pack);
	static void LoadMission(int curr_pack, int curr_miss);
	static std::pair<Mission*, NodeGraph*> LoadMission(std::string& path);
	static void LoadListMission(int curr_pack);
	static void SaveStoryline(int curr_storyline);
	static void LoadStoryline(int curr_storyline);
	static void SaveVars(std::string missPack);
	static void LoadVars(std::string missPack);
};

