// from https://kirit.com/How%20C%2B%2B%20coroutines%20work/My%20first%20coroutine

#include <iostream>
#include <experimental/coroutine>
#include <memory>

template<typename T>
struct sync {
    std::shared_ptr<T> value;
    sync(std::shared_ptr<T> p)
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
        auto final_suspend() {
            std::cout << "Finished the coro" << std::endl;
            return std::experimental::suspend_never{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
    };
};

sync<int> answer() {
    std::cout << "Thinking deep thoughts..." << std::endl;
    co_return 42;
}

int main() {
    auto a = answer();
    std::cout << "Got a coroutine, let's get a value" << std::endl;
    auto v = a.get();
    std::cout << "And the coroutine value is: " << v << std::endl;

    return 0;
}
