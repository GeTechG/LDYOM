#pragma once

#include "Init.h"

class NodeGraph;

class MissionPlayer
{
public:
	static void start_mission(Mission* mission, NodeGraph* graph, bool clearSelf);
	static void start_storyline();
};

