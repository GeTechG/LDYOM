#pragma once
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <list>
#include <boost/context/continuation.hpp>

class running_coro {
    using coro_t = std::function<void()>;
public:
    running_coro(coro_t coroutine) : _coroutine(coroutine), is_completed(false) {};

    void operator()();

    boost::context::continuation context;
    std::chrono::time_point<std::chrono::steady_clock,
        std::chrono::milliseconds> wake_time;
    bool is_completed;
private:
    coro_t _coroutine;
};

class coro_wait {
public:
    using coro_t = std::function<void()>;

    coro_wait() = default;
    coro_wait(std::initializer_list<coro_t> coroutines);

    void add_to_queue(coro_t coroutine);

    size_t task_count();

    void process();

private:
    std::list<running_coro> _task_queue;
    std::recursive_mutex _mutex;
};

namespace this_coro::detail {
    extern std::map<std::thread::id, running_coro*> coroutines;
}

namespace this_coro {
    template <class _Clock, class _Duration>
    inline void wait_until(const std::chrono::time_point<_Clock, _Duration>& time) {
        volatile auto coro = this_coro::detail::coroutines[std::this_thread::get_id()];

        coro->wake_time = std::chrono::time_point_cast<std::chrono::milliseconds>(time);
        coro->context = coro->context.resume();
    };

    template <class _Rep, class _Period>
    inline void wait(const std::chrono::duration<_Rep, _Period>& time) {
        wait_until(std::chrono::steady_clock::now() + time);
    };

    typedef unsigned int DWORD;
    inline void wait(DWORD dwMilliseconds) {
        wait_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(dwMilliseconds));
    };

    inline void yield() {
        wait_until(std::chrono::steady_clock::now());
    }
}
