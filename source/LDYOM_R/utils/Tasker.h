#pragma once
#include "ktcoro_wait.hpp"

class Tasker
{
private:
	ktcoro_tasklist taskList_;

	Tasker() = default;
	Tasker(const Tasker&) = delete;
	Tasker& operator=(Tasker&) = delete;
public:
	static Tasker& getInstance() {
		static Tasker instance;
		return instance;
	}

	ktcoro_tasklist& list();
};

