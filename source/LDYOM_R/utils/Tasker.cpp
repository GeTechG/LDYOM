#include "Tasker.h"

#include "Logger.h"

std::map<std::string, ktwait>& Tasker::list() {
	return this->taskList_;
}

ktcoro_tasklist& Tasker::getKtcoroTaskList() {
	return ktcoroTaskList_;
}

void Tasker::process() {
	this->ktcoroTaskList_.process();
}

void Tasker::removeTask(const std::string name) {
	try {
		ktcoroTaskList_.remove_task(taskList_.at(name));
		taskList_.erase(name);
	} catch (std::exception e) {
		Logger::getInstance().log(e.what());
	}
}
