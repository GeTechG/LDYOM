#pragma once
#include "ktcoro_wait.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class TaskState { Running, Completed, Waiting, Removed };

class TaskManager {
  public:
	static TaskManager& instance();

	void shutdown();

	~TaskManager();

	TaskManager(const TaskManager&) = delete;
	TaskManager& operator=(const TaskManager&) = delete;

	template <typename Coroutine, typename... Args>
	bool addTask(const std::string& key, Coroutine coro, Args&&... coro_args) {
		if (tasks.find(key) != tasks.end()) {
			return false;
		}

		ktwait task = taskList->add_task(coro, std::forward<Args>(coro_args)...);

		tasks.emplace(key, std::move(task));
		taskStates[key] = TaskState::Running;

		return true;
	}

	bool hasTask(const std::string& key) const;

	std::optional<TaskState> getTaskState(const std::string& key) const;

	bool removeTask(const std::string& key);

	bool completeTask(const std::string& key);

	std::shared_ptr<ktcoro_tasklist> getTaskList() const;

	void processAll();

	bool waitFor(const std::string& waitingTaskKey, const std::string& awaitedTaskKey);

	std::unordered_map<std::string, TaskState> getAllTaskStates() const;

	bool areAllTasksCompleted() const;

	size_t getTaskCount(TaskState state = TaskState::Running) const;

	bool hasErrorTasks() const;

  private:
	TaskManager();

	void updateTaskStates();

	std::shared_ptr<ktcoro_tasklist> taskList;
	std::unordered_map<std::string, ktwait> tasks;
	std::unordered_map<std::string, TaskState> taskStates;
};