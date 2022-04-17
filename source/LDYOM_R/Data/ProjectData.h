#pragma once
#include "Actor.h"

#include <vector>

class ProjectData {
private:
	std::vector<Actor> actors_;

public:
	std::vector<Actor>& getActors();
	void createNewActor();
};
