module;
#include <atomic>
#include <chrono>
#include <doctest/doctest.h>
#include <functional>
#include <iostream>
#include <thread>

export module util;

constexpr long SLEEP_DUR { 100 };


export namespace util {
    auto hello() -> void {
        std::cout << "Hello" << std::endl;
    }

    auto load_animation(std::atomic_bool& stop_flag) -> void {
        std::cout << "[-]" << std::flush;
        while (not stop_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DUR));
            std::cout << "\b\b\\]" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DUR));
            std::cout << "\b\b|]" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DUR));
            std::cout << "\b\b/]" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DUR));
            std::cout << "\b\b-]" << std::flush;
        }

        std::cout << "\b\b\b" << std::flush;
    }
};

TEST_SUITE("util") {
    TEST_CASE("animation thread should stop upon signal") {
        // Given
        std::atomic_bool stop_flag = false;
        std::thread t { util::load_animation, std::ref(stop_flag) };

        // When
        stop_flag = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        t.join();

        // Them
        CHECK_FALSE(t.joinable());
    }
}
