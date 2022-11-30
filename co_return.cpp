#include <iostream>
#include <experimental/coroutine>
#include <memory>

template<typename T>
struct sync {
    std::shared_ptr<T> value;

    constexpr sync(std::shared_ptr<T> p)
            : value(p) {
        std::cout << "Created a sync object" << std::endl;
    }

    sync(const sync &s)
            : value(s.value) {
        std::cout << "Copied a sync object" << std::endl;
    }

    ~sync() {
        std::cout << "Sync gone" << std::endl;
    }

    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        return *value;
    }

    struct promise_type {
        std::shared_ptr<T> ptr;

        promise_type()
                : ptr(std::make_shared<T>()) {
            std::cout << "Promise created" << std::endl;
        }

        ~promise_type() {
            std::cout << "Promise died" << std::endl;
        }

        auto get_return_object() {
            std::cout << "Send back a sync" << std::endl;
            return ptr;
        }

        auto initial_suspend() {
            std::cout << "Started the coroutine, don't stop now!" << std::endl;
            return std::experimental::suspend_never{};
        }

        auto return_value(T v) {
            std::cout << "Got an answer of " << v << std::endl;
            *ptr = v;
            return std::experimental::suspend_never{};
        }

        auto yield_value(T value) {
            std::cout << "Yielded " << value << std::endl;
            return std::experimental::suspend_always{};
        }

        auto final_suspend() noexcept {
            std::cout << "Finished the coro" << std::endl;
            return std::experimental::suspend_never{};
        }

        void unhandled_exception() {
            std::exit(1);
        }
    };
};

sync<int> iota(int n = 0) {
    while (true)
        co_yield n++;
}

sync<int> wait() {
    for (;;) {
        sync<int>::promise_type promiseType;
        co_await promiseType.initial_suspend();
    }
}

int main() {
    auto a = iota();
    std::cout << "Got a coroutine, let's get a value" << std::endl;
    auto v = a.get();
    std::cout << "And the coroutine value is: " << v << std::endl;
    auto w = wait();
    std::cout << "And the coroutine value is: " << v << std::endl;

    return 0;
}
