#include "Tasker.h"

#include <ranges>

#include "Logger.h"
#include "easylogging/easylogging++.h"

std::map<std::string, ktwait>& Tasker::list() {
	return this->taskList_;
}

ktcoro_tasklist& Tasker::getKtcoroTaskList() {
	return ktcoroTaskList_;
}

void Tasker::Reset() {
	std::vector<std::string> keys;
	for (const auto &key : this->taskList_ | std::views::keys) {
		keys.push_back(key);
	}
	for (auto key : keys) {
		removeTask(key);
	}
}

void Tasker::process() {
	this->ktcoroTaskList_.process();
}

void Tasker::removeTask(const std::string &name) {
	try {
		ktcoroTaskList_.remove_task(taskList_.at(name));
		taskList_.erase(name);
	} catch (std::exception &e) {
		CLOG(ERROR, "LDYOM") << "Error delete task in Tasker, error: " << e.what();
	}
}
