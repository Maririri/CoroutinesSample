// from https://blog.panicsoftware.com/co_awaiting-coroutines/

#include <iostream>
#include <experimental/coroutine>
#include <cassert>

class resumable {
public:
    struct promise_type;
    using coro_handle = std::experimental::coroutine_handle<promise_type>;

    resumable(coro_handle handle) : handle_(handle) { assert(handle); }

    resumable(resumable &) = delete;

    resumable(resumable &&) = delete;

    bool resume() {
        if (not handle_.done())
            handle_.resume();
        return not handle_.done();
    }

    const char *recent_val();

    ~resumable() { handle_.destroy(); }
private:
    coro_handle handle_;

};

struct resumable::promise_type {
    using coro_handle = std::experimental::coroutine_handle<promise_type>;
    const char *string_ = nullptr;

    auto get_return_object() {
        return coro_handle::from_promise(*this);
    }

    auto initial_suspend() { return std::experimental::suspend_always(); }

    auto yield_value(const char *string) {
        string_ = string;
        return std::experimental::suspend_always();
    }

    void return_void() {}

    auto final_suspend() { return std::experimental::suspend_always(); }

    void unhandled_exception() { std::terminate(); }
};

const char *resumable::recent_val() { return handle_.promise().string_; }

const char* foo(const char* param){}

resumable foo() {
    while (true) {
        co_yield foo("abc");
        co_yield "Hello";
        co_yield "Coroutine";
    }
}

resumable bar() {
    for (;;) {
        resumable::promise_type promise;
        co_await promise.yield_value("fourty two");
    }
}

int main() {
    resumable res = foo();
    int i = 3;
    while (i--) {
        res.resume();
        std::cout << res.recent_val() << std::endl;
    }

    auto wait = bar();
    wait.recent_val();
}