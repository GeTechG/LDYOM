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
		// Создаем копию списка задач под блокировкой мьютекса
		list_type tasks_to_process;
		{
			std::lock_guard<std::mutex> lock(tasks_mutex);
			tasks_to_process = tasks;
		}

		for (auto& task : tasks_to_process) {
			if (std::chrono::steady_clock::now() > task.wake_time && task.handle) {
				try {
					task.handle();

					// Маркируем задачу как завершенную в оригинальном списке
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

					// Маркируем задачу как завершенную в оригинальном списке
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

					// Маркируем задачу как завершенную в оригинальном списке
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

		// Удаляем завершенные задачи под блокировкой мьютекса
		std::lock_guard<std::mutex> lock(tasks_mutex);
		tasks.remove_if([](auto& task) { return task.completed; });
	}

	void remove_task(ktwait& task);
	void complete_task(ktwait& task); // Добавляем новый метод для завершения задачи

  private:
	list_type tasks;
	std::mutex tasks_mutex; // Мьютекс для синхронизации доступа к списку задач
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
		// Если рекурсивный вызов вернул true (task была найдена и удалена где-то глубже)...
		waiter.destroy(); // ...то уничтожаем и текущего waiter-а, так как он ждал удаленную задачу.
		return true;      // Сообщаем об успехе.
	}
	return false;
}

// Новая функция для поиска и добавления задач, ожидаемых удаляемой задачей
inline void find_awaited_tasks(std::coroutine_handle<ktwait::promise_type> task,
                               std::vector<std::coroutine_handle<ktwait::promise_type>>& tasks_to_remove,
                               std::vector<void*>& visited_addresses, int depth = 0) {
	// Проверяем уровень глубины рекурсии для защиты от переполнения стека
	if (depth > 100) {
		LDYOM_ERROR("Possible cyclic dependency detected in coroutine tasks. Stopping recursion.");
		return;
	}

	// Проверяем, ожидает ли задача кого-то
	if (task && task.promise().waiter) {
		// Получаем ожидаемую задачу
		auto awaited_task = std::coroutine_handle<ktwait::promise_type>::from_address(task.promise().waiter.address());

		// Проверяем, не было ли уже посещено это задание (защита от циклических зависимостей)
		void* task_address = awaited_task.address();
		if (std::find(visited_addresses.begin(), visited_addresses.end(), task_address) != visited_addresses.end()) {
			LDYOM_WARN("Cyclic dependency detected in coroutine tasks!");
			return;
		}

		// Добавляем адрес текущей задачи в список посещенных
		visited_addresses.push_back(task_address);

		// Проверяем, не добавлена ли уже эта задача в список для удаления
		if (std::find_if(tasks_to_remove.begin(), tasks_to_remove.end(), [&awaited_task](const auto& item) {
				return item.address() == awaited_task.address();
			}) == tasks_to_remove.end()) {
			// Добавляем ожидаемую задачу в список для удаления
			tasks_to_remove.push_back(awaited_task);

			// Рекурсивно ищем задачи, ожидаемые найденной задачей
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
	// Создаем вектор для хранения задач, которые ожидает удаляемая задача
	std::vector<std::coroutine_handle<ktwait::promise_type>> awaited_tasks;
	std::vector<void*> visited_addresses;

	// Находим задачи, которые ожидает удаляемая задача
	if (auto coro_handle = std::coroutine_handle<ktwait::promise_type>::from_address(task.coro_handle.address())) {
		detail::find_awaited_tasks(coro_handle, awaited_tasks, visited_addresses);
	}

	// Блокируем список задач перед изменением
	std::lock_guard<std::mutex> lock(tasks_mutex);

	// Сначала удаляем задачи, которые ожидаются удаляемой задачей
	for (auto& awaited_task : awaited_tasks) {
		for (auto it = tasks.begin(); it != tasks.end();) {
			if (it->handle.address() == awaited_task.address()) {
				// Уничтожаем корутин и удаляем задачу из списка
				awaited_task.destroy();
				tasks.erase(it++);
				break;
			}
			++it;
		}
	}

	// Теперь обрабатываем основную удаляемую задачу и всех её ожидающих
	for (auto it = tasks.begin(); it != tasks.end();) {
		if (it->handle == task.coro_handle) {
			it->handle.destroy();
			tasks.erase(it++);
		} else if (auto coro_handle{std::coroutine_handle<ktwait::promise_type>::from_address(it->handle.address())}) {
			auto& tw = coro_handle.promise().waiter;
			if (detail::remove_task_recursively(tw, task.coro_handle)) {
				coro_handle.destroy();
				tasks.erase(it++);
			} else {
				++it;
			}
		} else {
			++it;
		}
	}

	// Очищаем handle в самой задаче, чтобы предотвратить double-free
	task.coro_handle = nullptr;
}

inline void ktcoro_tasklist::complete_task(ktwait& task) {
	// Проверяем, что указатель корутины действителен
	if (!task.coro_handle) {
		LDYOM_WARN("Attempted to complete a task with null coroutine handle");
		return;
	}

	// Временные переменные для хранения информации о задаче, которую нужно активировать
	std::coroutine_handle<> waiter_to_activate = nullptr;

	// Первый блок с блокировкой мьютекса для обработки задачи
	{
		std::lock_guard<std::mutex> lock(tasks_mutex);

		bool task_found = false;
		for (auto it = tasks.begin(); it != tasks.end(); ++it) {
			if (it->handle.address() == task.coro_handle.address()) {
				task_found = true;
				// Получаем доступ к promise задачи
				if (auto coro_handle =
				        std::coroutine_handle<ktwait::promise_type>::from_address(it->handle.address())) {
					// Проверяем, есть ли у задачи ожидающие задачи
					if (coro_handle.promise().waiter) {
						// Сохраняем ожидающую задачу для активации после освобождения мьютекса
						waiter_to_activate = coro_handle.promise().waiter;
						coro_handle.promise().waiter = nullptr;
					}
					// Уничтожаем корутин, так как он больше не нужен
					coro_handle.destroy();
				}
				// Помечаем задачу как завершённую для удаления в process()
				it->completed = true;
				it->handle = nullptr; // Очищаем указатель для предотвращения повторного использования
				break;
			}
		}

		if (!task_found) {
			LDYOM_WARN("Attempted to complete a task that was not found in the task list");
		}
	}

	// Вне блокировки активируем ожидающую задачу, если она есть
	if (waiter_to_activate) {
		try {
			add_started_task(waiter_to_activate,
			                 std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()));
		} catch (const std::exception& e) {
			LDYOM_ERROR("Exception when scheduling awaiting task: {}", e.what());
		}
	}

	// Очищаем handle в самой задаче, чтобы предотвратить double-free
	task.coro_handle = nullptr;
}