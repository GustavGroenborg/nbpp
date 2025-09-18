module;
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <unistd.h>

export module nbpp;

import parser;
import util;

export namespace nbpp {
    /**
     * Runs the application nbpp
     */
    auto run() -> void {
        util::hello();

        std::atomic_bool animation_stop_flag = false;
        std::thread t { util::load_animation, std::ref(animation_stop_flag) };

        sleep(5);
        animation_stop_flag = true;
        sleep(1);
        t.join();
        std::cout << "animation thread joinable: " << (t.joinable() == false) << std::endl;
    }
}
