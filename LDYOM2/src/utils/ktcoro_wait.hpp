#pragma once
#include <chrono>
#include <coroutine>
#include <list>
#include <logger.h>
#include <mutex>
#include <utility>
#include <vector>

namespace detail {
struct ctx {
	std::coroutine_handle<> handle;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> wake_time;
	bool completed{false};

	ctx(std::coroutine_handle<> h, std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> t)
		: handle(h),
		  wake_time(t) {}
};
} // namespace detail

struct ktwait;

class ktcoro_tasklist {
	using list_type = std::list<detail::ctx>;

  public:
	template <typename Coroutine, typename... Args> ktwait add_task(Coroutine coro, Args&&... coro_args);

	void add_started_task(std::coroutine_handle<> coro,
	                      std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> time) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		tasks.emplace_back(coro, time);
	}

	void process() {

		processDeferredOperations();

		list_type tasks_to_process;
		{
			std::lock_guard<std::mutex> lock(tasks_mutex);
			tasks_to_process = tasks;
		}

		for (auto& task : tasks_to_process) {
			if (std::chrono::steady_clock::now() > task.wake_time && task.handle) {
				try {
					task.handle();

					std::lock_guard<std::mutex> lock(tasks_mutex);
					auto original_task_it = std::find_if(tasks.begin(), tasks.end(), [&task](const auto& t) {
						return t.handle.address() == task.handle.address();
					});

					if (original_task_it != tasks.end()) {
						original_task_it->completed = true;
					}
				} catch (const std::exception& e) {
					LDYOM_ERROR("Exception in coroutine task: {}", e.what());
					LDYOM_DUMP_BACKTRACE();

					std::lock_guard<std::mutex> lock(tasks_mutex);
					auto original_task_it = std::find_if(tasks.begin(), tasks.end(), [&task](const auto& t) {
						return t.handle.address() == task.handle.address();
					});

					if (original_task_it != tasks.end()) {
						original_task_it->completed = true;
					}
				} catch (...) {
					LDYOM_CRITICAL("Unknown exception in coroutine task");
					LDYOM_DUMP_BACKTRACE();

					std::lock_guard<std::mutex> lock(tasks_mutex);
					auto original_task_it = std::find_if(tasks.begin(), tasks.end(), [&task](const auto& t) {
						return t.handle.address() == task.handle.address();
					});

					if (original_task_it != tasks.end()) {
						original_task_it->completed = true;
					}
				}
			}
		}

		{
			std::lock_guard<std::mutex> lock(tasks_mutex);
			tasks.remove_if([](auto& task) { return task.completed; });
		}

		processDeferredOperations();
	}

	void remove_task(ktwait& task);
	void complete_task(ktwait& task);

	void completed(void* task_addr) {
		{
			std::lock_guard<std::mutex> lock(deferred_ops_mutex);
			auto it = std::find(tasks_to_remove.begin(), tasks_to_remove.end(), task_addr);
			if (it != tasks_to_remove.end()) {
				tasks_to_remove.erase(it);
			}
		}
		{
			std::lock_guard<std::mutex> lock(deferred_ops_mutex);
			auto it = std::find(tasks_to_complete.begin(), tasks_to_complete.end(), task_addr);
			if (it != tasks_to_complete.end()) {
				tasks_to_complete.erase(it);
			}
		}
	}

  private:
	inline void do_remove_task(void* task_addr);
	inline void do_complete_task(void* task_addr);
	inline void processDeferredOperations();

	list_type tasks;
	std::mutex tasks_mutex;

	std::vector<void*> tasks_to_remove;
	std::vector<void*> tasks_to_complete;
	std::mutex deferred_ops_mutex;
};

struct ktwait {
	class promise_type {
	  public:
		void return_void() const {}

		auto initial_suspend() const { return std::suspend_always{}; }

		auto final_suspend() noexcept {
			struct execute_on_suspend {
				promise_type* me;

				constexpr bool await_ready() const noexcept { return true; }
				constexpr void await_suspend(std::coroutine_handle<promise_type> h) const noexcept {}

				void await_resume() const noexcept {
					if (me->coro_tasklist) {
						auto h = std::coroutine_handle<promise_type>::from_promise(*me);
						me->coro_tasklist->completed(h.address());
					}
					if (me->waiter)
						me->waiter();
				}
			};
			return execute_on_suspend{this};
		}

		void unhandled_exception() {
			try {
				std::rethrow_exception(std::current_exception());
			} catch (const std::exception& e) {
				LDYOM_ERROR("Unhandled exception in coroutine: {}", e.what());
				LDYOM_DUMP_BACKTRACE();
			} catch (...) {
				LDYOM_CRITICAL("Unknown unhandled exception in coroutine");
				LDYOM_DUMP_BACKTRACE();
			}
		}

		auto get_return_object() { return ktwait{std::coroutine_handle<promise_type>::from_promise(*this)}; }

		template <class Clock, class Duration> auto yield_value(const std::chrono::time_point<Clock, Duration>& time) {
			struct schedule_for_execution {
				ktcoro_tasklist* wq;
				std::chrono::time_point<Clock, std::chrono::milliseconds> t;

				constexpr bool await_ready() const noexcept { return false; }
				void await_suspend(std::coroutine_handle<> this_coro) const { wq->add_started_task(this_coro, t); }
				constexpr void await_resume() const noexcept {}
			};
			return schedule_for_execution{coro_tasklist, std::chrono::time_point_cast<std::chrono::milliseconds>(time)};
		}

		auto yield_value() { return yield_value(std::chrono::steady_clock::now()); }

		template <class Rep, class Period> auto await_transform(const std::chrono::duration<Rep, Period>& time) {
			return yield_value(std::chrono::steady_clock::now() + time);
		}

		auto await_transform(unsigned long msecs) {
			return yield_value(std::chrono::steady_clock::now() + std::chrono::milliseconds(msecs));
		}

		auto await_transform(ktwait waitobj) {
			struct execute_on_await {
				ktwait waitobj;

				constexpr bool await_ready() const noexcept { return false; }

				void await_suspend(std::coroutine_handle<promise_type> h) const {
					namespace c = std::chrono;
					auto& called_by = waitobj.coro_handle.promise();
					called_by.waiter = h;
					called_by.coro_tasklist = h.promise().coro_tasklist;
					called_by.coro_tasklist->add_started_task(
						waitobj.coro_handle, c::time_point_cast<c::milliseconds>(c::steady_clock::now()));
				}

				void await_resume() const noexcept {}
			};
			return execute_on_await{std::move(waitobj)};
		}

		std::coroutine_handle<promise_type> waiter;
		ktcoro_tasklist* coro_tasklist;
	};

	ktwait(std::coroutine_handle<promise_type> h)
		: coro_handle(h) {}

	ktwait(ktwait&&) = default;

	std::coroutine_handle<promise_type> coro_handle;
};

namespace detail {
inline bool remove_task_recursively(std::coroutine_handle<ktwait::promise_type>& waiter,
                                    std::coroutine_handle<ktwait::promise_type>& task) {
	if (!waiter)
		return false;
	if (waiter == task) {
		task.destroy();
		return true;
	}
	if (remove_task_recursively(waiter.promise().waiter, task)) {

		waiter.destroy();
		return true;
	}
	return false;
}

inline void find_awaited_tasks(std::coroutine_handle<ktwait::promise_type> task,
                               std::vector<std::coroutine_handle<ktwait::promise_type>>& tasks_to_remove,
                               std::vector<void*>& visited_addresses, int depth = 0) {

	if (depth > 100) {
		LDYOM_ERROR("Possible cyclic dependency detected in coroutine tasks. Stopping recursion.");
		return;
	}

	if (task && task.promise().waiter) {

		auto awaited_task = std::coroutine_handle<ktwait::promise_type>::from_address(task.promise().waiter.address());
		void* task_address = awaited_task.address();
		if (std::find(visited_addresses.begin(), visited_addresses.end(), task_address) != visited_addresses.end()) {
			LDYOM_WARN("Cyclic dependency detected in coroutine tasks!");
			return;
		}
		visited_addresses.push_back(task_address);
		if (std::find_if(tasks_to_remove.begin(), tasks_to_remove.end(), [&awaited_task](const auto& item) {
				return item.address() == awaited_task.address();
			}) == tasks_to_remove.end()) {
			tasks_to_remove.push_back(awaited_task);
			find_awaited_tasks(awaited_task, tasks_to_remove, visited_addresses, depth + 1);
		}
	}
}
} // namespace detail

template <typename Coroutine, typename... Args> ktwait ktcoro_tasklist::add_task(Coroutine coro, Args&&... coro_args) {
	namespace c = std::chrono;
	ktwait task{std::move(coro(std::forward<Args>(coro_args)...))};
	task.coro_handle.promise().coro_tasklist = this;
	add_started_task(task.coro_handle, c::time_point_cast<c::milliseconds>(c::steady_clock::now()));
	return task;
}

inline void ktcoro_tasklist::remove_task(ktwait& task) {

	if (!task.coro_handle) {
		LDYOM_WARN("Attempted to remove a task with null coroutine handle");
		return;
	}

	void* addr = task.coro_handle.address();
	{
		std::lock_guard<std::mutex> lock(deferred_ops_mutex);
		if (std::find(tasks_to_remove.begin(), tasks_to_remove.end(), addr) == tasks_to_remove.end()) {
			tasks_to_remove.push_back(addr);
		}
	}

	task.coro_handle = nullptr;
}

inline void ktcoro_tasklist::complete_task(ktwait& task) {

	if (!task.coro_handle) {
		LDYOM_WARN("Attempted to complete a task with null coroutine handle");
		return;
	}

	void* addr = task.coro_handle.address();
	{
		std::lock_guard<std::mutex> lock(deferred_ops_mutex);
		if (std::find(tasks_to_complete.begin(), tasks_to_complete.end(), addr) == tasks_to_complete.end()) {
			tasks_to_complete.push_back(addr);
		}
	}

	task.coro_handle = nullptr;
}

inline void ktcoro_tasklist::do_remove_task(void* task_addr) {
	if (!task_addr) {
		return;
	}

	std::vector<std::coroutine_handle<ktwait::promise_type>> awaited_tasks;
	std::vector<void*> visited_addresses;

	if (auto coro_handle = std::coroutine_handle<ktwait::promise_type>::from_address(task_addr)) {
		if (!coro_handle.done()) {
			detail::find_awaited_tasks(coro_handle, awaited_tasks, visited_addresses);
		}
	}

	std::lock_guard<std::mutex> tasks_lock(tasks_mutex);

	for (auto& awaited_task : awaited_tasks) {
		if (awaited_task && !awaited_task.done()) {
			for (auto it = tasks.begin(); it != tasks.end();) {
				if (it->handle.address() == awaited_task.address()) {

					awaited_task.destroy();
					tasks.erase(it++);
					break;
				}
				++it;
			}
		}
	}

	for (auto it = tasks.begin(); it != tasks.end();) {
		if (it->handle.address() == task_addr) {
			auto handle = it->handle;
			if (handle) {
				handle.destroy();
			}
			tasks.erase(it++);
		} else if (auto coro_handle{std::coroutine_handle<ktwait::promise_type>::from_address(it->handle.address())}) {
			auto& tw = coro_handle.promise().waiter;
			auto task = std::coroutine_handle<ktwait::promise_type>::from_address(task_addr);
			if (detail::remove_task_recursively(tw, task)) {
				coro_handle.destroy();
				tasks.erase(it++);
			} else if (tw && tw.address() == task_addr) {
				tw = nullptr;
				++it;
			} else {
				++it;
			}
		} else {
			++it;
		}
	}
}

inline void ktcoro_tasklist::do_complete_task(void* task_addr) {
	if (!task_addr) {
		return;
	}

	std::coroutine_handle<> waiter_to_activate = nullptr;
	bool task_found = false;

	{
		std::lock_guard<std::mutex> tasks_lock(tasks_mutex);
		for (auto it = tasks.begin(); it != tasks.end(); ++it) {
			if (it->handle.address() == task_addr) {
				task_found = true;

				if (auto coro_handle =
				        std::coroutine_handle<ktwait::promise_type>::from_address(it->handle.address())) {

					if (!coro_handle.done()) {

						if (coro_handle.promise().waiter) {

							waiter_to_activate = coro_handle.promise().waiter;
							coro_handle.promise().waiter = nullptr;
						}

						coro_handle.destroy();
					} else {
						LDYOM_WARN("Attempted to complete an already done coroutine");
					}
				}

				it->completed = true;
				it->handle = nullptr;
				break;
			}
		}
	}

	if (waiter_to_activate) {
		try {
			add_started_task(waiter_to_activate,
			                 std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()));
		} catch (const std::exception& e) {
			LDYOM_ERROR("Exception when scheduling awaiting task: {}", e.what());
		}
	}

	if (!task_found) {
		LDYOM_WARN("Deferred completion: Task not found in the task list");
	}
}

inline void ktcoro_tasklist::processDeferredOperations() {
	std::vector<void*> remove_addrs;
	std::vector<void*> complete_addrs;

	{
		std::lock_guard<std::mutex> deferred_lock(deferred_ops_mutex);
		remove_addrs = std::move(tasks_to_remove);
		complete_addrs = std::move(tasks_to_complete);
		tasks_to_remove.clear();
		tasks_to_complete.clear();
	}

	for (void* addr : complete_addrs) {
		do_complete_task(addr);
	}

	for (void* addr : remove_addrs) {
		do_remove_task(addr);
	}
}