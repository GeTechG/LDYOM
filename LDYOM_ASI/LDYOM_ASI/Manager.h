#pragma once

class Manager
{
public:
	static void SaveMission(int curr_pack, int curr_miss);
	static void SaveListMission(int curr_pack);
	static void LoadMission(int curr_pack, int curr_miss);
	static void LoadListMission(int curr_pack);
};

