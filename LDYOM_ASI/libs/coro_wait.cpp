#include "coro_wait.h"

namespace ctx = boost::context;

void running_coro::operator()() {
    this_coro::detail::coroutines[std::this_thread::get_id()] = this;

    if (context) {
        context = context.resume();
    }
    else {
        context = boost::context::callcc(
            [&](ctx::continuation&& sink) {
            context = std::move(sink);

            _coroutine();

            is_completed = true;

            return std::move(context);
        });
    }
}

coro_wait::coro_wait(std::initializer_list<coro_t> coroutines) {
    for (auto coro : coroutines) {
        _task_queue.emplace_back(coro);
    }
}

void coro_wait::add_to_queue(coro_t coroutine) {
    std::lock_guard guard(_mutex);

    _task_queue.emplace_back(coroutine);
}

size_t coro_wait::task_count() {
    std::lock_guard guard(_mutex);

    return std::count_if(_task_queue.begin(), _task_queue.end(),
        [](const running_coro& a) {
        return !a.is_completed;
    });
}

void coro_wait::process() {
    std::lock_guard guard(_mutex);

    for (auto& task : _task_queue) {
        if (std::chrono::steady_clock::now() > task.wake_time)
            task();
    }

    _task_queue.remove_if(
        [](const running_coro& a) {
        return a.is_completed;
    });
}

namespace this_coro::detail {
    std::map<std::thread::id, running_coro*> coroutines;
}
