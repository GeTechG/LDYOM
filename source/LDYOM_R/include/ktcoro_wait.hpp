#ifndef KTCORO_WAIT_HPP_
#define KTCORO_WAIT_HPP_

#include <coroutine>
#include <list>
#include <chrono>
#include <utility>

namespace detail {
    struct ctx {
        std::coroutine_handle<> handle;
        std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> wake_time;
        bool completed{ false };

        ctx(std::coroutine_handle<> h,
            std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> t) :
            handle(h), wake_time(t) {}
    };
}

struct ktwait;

class ktcoro_tasklist {
    using list_type = std::list<detail::ctx>;
public:
    template <typename Coroutine, typename... Args>
    ktwait add_task(Coroutine coro, Args&&... coro_args);

    void add_started_task(std::coroutine_handle<> coro, std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> time) {
        tasks.emplace_back(coro, time);
    }

    void process() {
        for (auto& task : tasks) {
            if (std::chrono::steady_clock::now() > task.wake_time && task.handle) {
                task.handle();
                task.completed = true;
            }
        }
        tasks.remove_if([](auto& task) { return task.completed; });
    }

    void remove_task(ktwait& task);
private:
    list_type tasks;
};

struct ktwait {
    class promise_type {
    public:
        void return_void() const {}

        auto initial_suspend() const {
            return std::suspend_always{};
        }

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
            return execute_on_suspend{ this };
        }

        void unhandled_exception() const {}

        auto get_return_object() {
            return ktwait{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        template <class Clock, class Duration>
        auto yield_value(const std::chrono::time_point<Clock, Duration>& time) {
            struct schedule_for_execution {
                ktcoro_tasklist* wq;
                std::chrono::time_point<Clock,
                    std::chrono::milliseconds> t;

                constexpr bool await_ready() const noexcept { return false; }
                void await_suspend(std::coroutine_handle<> this_coro) const {
                    wq->add_started_task(this_coro, t);
                }
                constexpr void await_resume() const noexcept {}
            };
            return schedule_for_execution{ coro_tasklist, std::chrono::time_point_cast<std::chrono::milliseconds>(time) };
        }

        auto yield_value() {
            return yield_value(std::chrono::steady_clock::now());
        }

        template <class Rep, class Period>
        auto await_transform(const std::chrono::duration<Rep, Period>& time) {
            return yield_value(std::chrono::steady_clock::now() + time);
        }

        auto await_transform(unsigned long msecs) {
            return yield_value(std::chrono::steady_clock::now() + std::chrono::milliseconds(msecs));
        }

        auto await_transform(ktwait waitobj) {
            struct execute_on_await {
                ktwait waitobj;

                constexpr bool await_ready() const noexcept {
                    return false;
                }
                void await_suspend(std::coroutine_handle<promise_type> h) const {
                    namespace c = std::chrono;
                    auto& called_by = waitobj.coro_handle.promise();
                    called_by.waiter = h;
                    called_by.coro_tasklist = h.promise().coro_tasklist;
                    called_by.coro_tasklist->add_started_task(waitobj.coro_handle, c::time_point_cast<c::milliseconds>(c::steady_clock::now()));
                }
                void await_resume() const noexcept { }
            };
            return execute_on_await{ std::move(waitobj) };
        }

        std::coroutine_handle<promise_type> waiter;
        ktcoro_tasklist* coro_tasklist;
    };

    ktwait(std::coroutine_handle<promise_type> h) : coro_handle(h) {}
    ktwait(ktwait&&) = default;

    std::coroutine_handle<promise_type> coro_handle;
};

namespace detail {
    inline bool remove_task_recursively(std::coroutine_handle<ktwait::promise_type>& waiter, std::coroutine_handle<ktwait::promise_type>& task) {
        if (!waiter) return false;
        if (waiter == task) { task.destroy(); return true; }
        if (remove_task_recursively(waiter.promise().waiter, task)) {
            waiter.destroy();
            return true;
        }
        return false;
    }
}

template<typename Coroutine, typename ...Args>
inline ktwait ktcoro_tasklist::add_task(Coroutine coro, Args && ...coro_args) {
    namespace c = std::chrono;
    ktwait task{ std::move(coro(std::forward<Args>(coro_args)...)) };
    task.coro_handle.promise().coro_tasklist = this;
    add_started_task(task.coro_handle, c::time_point_cast<c::milliseconds>(c::steady_clock::now()));
    return task;
}

inline void ktcoro_tasklist::remove_task(ktwait& task) {
    for (auto it = tasks.begin(); it != tasks.end();) {
        if (it->handle == task.coro_handle) {
            it->handle.destroy();
            tasks.erase(it++);
        }
        else if (auto coro_handle{ std::coroutine_handle<ktwait::promise_type>::from_address(it->handle.address()) }) {
            auto& tw = coro_handle.promise().waiter;
            if (detail::remove_task_recursively(tw, task.coro_handle)) {
                coro_handle.destroy();
                tasks.erase(it++);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

#endif // KTCORO_WAIT_HPP_
