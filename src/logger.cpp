module;
#include <spdlog/spdlog.h>
#include <format>

export module quickfix_logger;

/**
 * NOTE: This is a duct tape class, to fix logging, while argument with
 * spdlog is broken. 
 */
export namespace qlog {
    template<class... Args>
    auto error(std::format_string<Args...> fmt, Args&&... args) -> void {
        spdlog::error(std::format(fmt, args...));
    }

    template<class... Args>
    auto warn(std::format_string<Args...> fmt, Args&&... args) -> void {
        spdlog::warn(std::format(fmt, args...));
    }

    template<class... Args>
    auto info(std::format_string<Args...> fmt, Args&&... args) -> void {
        spdlog::info(std::format(fmt, args...));
    }
}
