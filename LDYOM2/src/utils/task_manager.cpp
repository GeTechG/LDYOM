#include "task_manager.h"
#include <coroutine>

TaskManager& TaskManager::instance() {
	static TaskManager instance;
	return instance;
}

TaskManager::TaskManager()
	: taskList(std::make_shared<ktcoro_tasklist>()) {}

TaskManager::~TaskManager() {}

void TaskManager::shutdown() {

	for (auto it = tasks.begin(); it != tasks.end();) {
		auto key = it->first;
		++it;
		removeTask(key);
	}

	tasks.clear();
	taskStates.clear();
}

bool TaskManager::hasTask(const std::string& key) const { return tasks.find(key) != tasks.end(); }

std::optional<TaskState> TaskManager::getTaskState(const std::string& key) const {
	auto stateIt = taskStates.find(key);
	if (stateIt != taskStates.end()) {
		return stateIt->second;
	}
	return std::nullopt;
}

bool TaskManager::removeTask(const std::string& key) {
	auto taskIt = tasks.find(key);
	if (taskIt != tasks.end()) {

		taskList->remove_task(taskIt->second);

		taskStates[key] = TaskState::Removed;

		tasks.erase(taskIt);

		return true;
	}
	return false;
}

bool TaskManager::completeTask(const std::string& key) {
	auto taskIt = tasks.find(key);
	if (taskIt != tasks.end()) {

		taskList->complete_task(taskIt->second);

		taskStates[key] = TaskState::Completed;

		tasks.erase(taskIt);

		return true;
	}
	return false;
}

std::shared_ptr<ktcoro_tasklist> TaskManager::getTaskList() const { return taskList; }

void TaskManager::processAll() {
	taskList->process();

	updateTaskStates();
}

bool TaskManager::waitFor(const std::string& waitingTaskKey, const std::string& awaitedTaskKey) {
	auto waitingTaskIt = tasks.find(waitingTaskKey);
	auto awaitedTaskIt = tasks.find(awaitedTaskKey);

	if (waitingTaskIt != tasks.end() && awaitedTaskIt != tasks.end()) {

		auto& waitingTask = waitingTaskIt->second;
		auto& awaitedTask = awaitedTaskIt->second;

		if (auto waitingHandle =
		        std::coroutine_handle<ktwait::promise_type>::from_address(waitingTask.coro_handle.address())) {
			if (auto awaitedHandle =
			        std::coroutine_handle<ktwait::promise_type>::from_address(awaitedTask.coro_handle.address())) {

				waitingHandle.promise().waiter = nullptr;
				awaitedHandle.promise().waiter = waitingHandle;

				taskStates[waitingTaskKey] = TaskState::Waiting;

				return true;
			}
		}
	}
	return false;
}

void TaskManager::updateTaskStates() {
	std::vector<std::string> keysToRemove;

	for (auto it = taskStates.begin(); it != taskStates.end(); ++it) {
		const std::string& key = it->first;

		auto taskIt = tasks.find(key);
		if (taskIt == tasks.end()) {
			continue;
		}

		auto& task = taskIt->second;
		if (auto coro_handle = std::coroutine_handle<ktwait::promise_type>::from_address(task.coro_handle.address())) {
			if (coro_handle.promise().waiter) {
				taskStates[key] = TaskState::Waiting;
			} else {
				taskStates[key] = TaskState::Running;
			}

			if (coro_handle.done() || taskList->deleted_tasks.contains(coro_handle.address())) {
				taskStates[key] = TaskState::Completed;
				keysToRemove.push_back(key);
				taskList->deleted_tasks.erase(coro_handle.address());
			}
		}
	}

	for (const auto& key : keysToRemove) {
		tasks.erase(key);
	}
}

std::unordered_map<std::string, TaskState> TaskManager::getAllTaskStates() const { return taskStates; }

bool TaskManager::areAllTasksCompleted() const {
	for (const auto& pair : taskStates) {
		if (pair.second != TaskState::Completed && pair.second != TaskState::Removed) {
			return false;
		}
	}
	return true;
}

size_t TaskManager::getTaskCount(TaskState state) const {
	size_t count = 0;
	for (const auto& pair : taskStates) {
		if (pair.second == state) {
			count++;
		}
	}
	return count;
}

bool TaskManager::hasErrorTasks() const { return false; }