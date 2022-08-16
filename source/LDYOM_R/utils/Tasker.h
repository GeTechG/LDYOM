#pragma once
#include <map>

#include "ktcoro_wait.hpp"

class Tasker
{
private:
	ktcoro_tasklist ktcoroTaskList_;
	std::map<std::string, ktwait> taskList_;

	Tasker() = default;
	Tasker(const Tasker&) = delete;
	Tasker& operator=(Tasker&) = delete;
public:
	static Tasker& getInstance() {
		static Tasker instance;
		return instance;
	}

	std::map<std::string, ktwait>& list();
	ktcoro_tasklist& getKtcoroTaskList();

	void process();

	template<typename Coroutine, typename ...Args>
	ktwait& addTask(std::string name, Coroutine coro, Args && ...coro_args);
	void removeTask(const std::string& name);
};

template <typename Coroutine, typename ... Args>
ktwait& Tasker::addTask(std::string name, Coroutine coro, Args&&... coro_args) {
	ktwait iKtwait = ktcoroTaskList_.add_task(coro, coro_args...);
	const auto result = taskList_.emplace(name, std::move(iKtwait));
	return result.first->second;
}

