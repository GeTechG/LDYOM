#ifndef KTSIGNAL_HPP
#define KTSIGNAL_HPP

#include <functional>
#include <list>
#include <shared_mutex>
#include <mutex>
#include <tuple>
#include <iterator>

namespace ktsignal {
    struct empty_mutex {
        void lock() {}
        void unlock() {}

        void lock_shared() {}
        void unlock_shared() {}
    };
    template <typename FuncSig, bool thread_safe = true, bool emit_thread_safe = false>
    class ktsignal_impl
    {
        using mutex_type = std::conditional_t<thread_safe, std::shared_mutex, empty_mutex>;
        using emit_locker = std::conditional_t<emit_thread_safe, std::unique_lock<mutex_type>, std::shared_lock<mutex_type>>;
    public:
        using slot_type = std::function<FuncSig>;
        using slot_container = std::list<slot_type>;
        using slot_iterator = typename slot_container::iterator;

        class ktsignal_connection
        {
        public:
            void disconnect() {
                erase_func();
                erase_func = []() {};
            }
            ktsignal_connection(std::function<void()> func) : erase_func(func) {};
            ktsignal_connection(ktsignal_connection&) = delete;
            ktsignal_connection(ktsignal_connection&&) = default;

            ktsignal_connection& operator=(ktsignal_connection&) = delete;
            ktsignal_connection& operator=(ktsignal_connection&&) = default;

            ~ktsignal_connection() = default;
        private:
            std::function<void()> erase_func;
        };

        class ktsignal_scoped_connection : public ktsignal_connection
        {
        public:
            ~ktsignal_scoped_connection() { ktsignal_connection::disconnect(); }
            ktsignal_scoped_connection(std::function<void()> func) : ktsignal_connection(func) {};
            ktsignal_scoped_connection(ktsignal_scoped_connection&) = delete;
            ktsignal_scoped_connection(ktsignal_scoped_connection&& Right) = default;

            ktsignal_scoped_connection& operator=(ktsignal_scoped_connection&) = delete;
            ktsignal_scoped_connection& operator=(ktsignal_scoped_connection&&) = default;
        };

        ktsignal_connection connect(slot_type slot) {
            std::unique_lock<mutex_type> lock(slots_mutex);

            auto erase_iter = slots_.emplace(slots_.end(), std::move(slot));

            std::function<void()> lambda = [&slots_mutex = this->slots_mutex, &slots_ = this->slots_, erase_iter]() -> void {
                std::unique_lock<mutex_type> lock(slots_mutex);
                slots_.erase(erase_iter);
            };
            return ktsignal_connection{ std::move(lambda) };
        }
        ktsignal_scoped_connection scoped_connect(slot_type slot) {
            std::unique_lock<mutex_type> lock(slots_mutex);

            auto erase_iter = slots_.emplace(slots_.end(), std::move(slot));

            std::function<void()> lambda = [&slots_mutex = this->slots_mutex, &slots_ = this->slots_, erase_iter]() -> void {
                std::unique_lock<mutex_type> lock(slots_mutex);
                slots_.erase(erase_iter);
            };
            return ktsignal_scoped_connection{ std::move(lambda) };
        }
        
        template <class C, typename R, typename... Args>
        ktsignal_connection connect(C* ptr, R(C::* func)(Args...)) {
            return connect([ptr, func](Args&&... args) -> R { return (ptr->*func)(std::forward<Args>(args)...); });
        }
        template <class C, typename R, typename... Args>
        ktsignal_scoped_connection scoped_connect(C* ptr, R(C::* func)(Args...)) {
            return scoped_connect([ptr, func](Args&&... args) -> R { return (ptr->*func)(std::forward<Args>(args)...); });
        }

        ktsignal_impl& operator+=(slot_type slot) {
            connect(slot);
            return *this;
        }
        
        template <typename... Args>
        void emit(Args&&... args) {
            emit_locker lock(slots_mutex);
            for (auto& f : slots_) {
                f(std::forward<Args>(args)...);
            }
        }

        template <typename... CallArgs>
        class signal_iterator {

#if __cpp_lib_apply < 201603L
            template <int... Is>
            struct index {};

            template <int N, int... Is>
            struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

            template <int... Is>
            struct gen_seq<0, Is...> : index<Is...> {};

            template <typename... Ts, int... Is>
            auto iter_call(slot_type slot, std::tuple<Ts...>& tup, index<Is...>)
            {
                return slot(std::get<Is>(tup)...);
            }

            template <typename... Ts>
            auto iter_call(slot_type slot, std::tuple<Ts...>& tup)
            {
                return iter_call(slot, tup, gen_seq<sizeof...(Ts)>{});
            }
#endif

        public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = typename slot_type::result_type;
            using pointer = typename slot_type::result_type*;
            using reference = typename slot_type::result_type&;

            signal_iterator(slot_iterator iter, mutex_type& slots_mutex, std::tuple<CallArgs&&...>& tup) :
                slots_mutex_(slots_mutex), iter_(iter),
                stored_args(tup) {}

            value_type operator*() {
                emit_locker lock(slots_mutex_);
#if __cpp_lib_apply >= 201603L
                return std::apply(*iter_, stored_args);
#else
                return iter_call(*iter_, stored_args);
#endif
            }

            signal_iterator& operator++() { iter_++; return *this; }

            signal_iterator operator++(int) { signal_iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const signal_iterator& a, const signal_iterator& b) { return a.iter_ == b.iter_; }
            friend bool operator!= (const signal_iterator& a, const signal_iterator& b) { return a.iter_ != b.iter_; }
        private:
            slot_iterator iter_;
            mutex_type& slots_mutex_;
            std::tuple<CallArgs&&...>& stored_args;
        };

        template <typename... CallArgs>
        class iterate {
        public:
            iterate(std::list<slot_type>& slots, mutex_type& slots_mutex, CallArgs&&... args) :
                slots_(slots), slots_mutex_(slots_mutex), stored_args(std::forward<CallArgs>(args)...) {}

            auto begin() {
                return signal_iterator<CallArgs...>(slots_.begin(), slots_mutex_, stored_args);
            }
            auto end() {
                return signal_iterator<CallArgs...>(slots_.end(), slots_mutex_, stored_args);
            }
        private:
            std::tuple<CallArgs&&...> stored_args;
            std::list<slot_type>& slots_;
            mutex_type& slots_mutex_;
        };

        template <typename... Args>
        iterate<Args...> emit_iterate(Args&&... args) {
            return iterate<Args...>(slots_, slots_mutex, std::forward<Args>(args)...);
        }

    private:
        std::list<slot_type> slots_;
        mutex_type slots_mutex;
    };

    template <typename FuncSig>
    using ktsignal = ktsignal_impl<FuncSig, false, false>;
    template <typename FuncSig>
    using ktsignal_threadsafe = ktsignal_impl<FuncSig, true, false>;
    template <typename FuncSig>
    using ktsignal_threadsafe_emit = ktsignal_impl<FuncSig, true, true>;
}

#endif // KTSIGNAL_HPP
