#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <string>
#include <sstream>

import nbpp;

constexpr std::string DOCTEST_PREFIX { "--dt-" };

auto clean_args(int argc, const char *argv[]) -> std::string;
auto has_prefix(std::string_view prefix, const char str[]) -> bool;

auto main(int argc, char *argv[]) -> int {
    for (auto i = 0; i < argc; i++) {
        std::cout << argv[i] << ' ' << '\n';
    }
    doctest::Context ctx;

    ctx.applyCommandLine(argc, argv);

    // Do not break debugger if assertion fails
    ctx.setOption("no-breaks", true);

    // Running the tests
    int res = ctx.run();

    if (ctx.shouldExit())
        return res; // Propagating result of tests
    
    nbpp::run();

    return EXIT_SUCCESS;
}


auto clean_args(int argc, const char *argv[]) -> std::string {
    std::ostringstream stream { std::string(argv[0]), std::ios_base::ate };
    for (auto i = 1; i < argc; i++) {
        if (!has_prefix(DOCTEST_PREFIX, argv[i])) {
            stream << ' ' << argv[i];
        }
    }

    return stream.str();
}

auto has_prefix(std::string_view prefix, const char str[]) -> bool {
    auto prefix_iter { prefix.cbegin() };
    size_t idx = 0;
    while (prefix_iter != prefix.cend() and str[idx] != '\0') {
        if (*prefix_iter == str[idx]) {
            ++prefix_iter;
            ++idx;
        } else {
            return false;
        }
    }

    return prefix_iter == prefix.cend();
}

TEST_SUITE("test-setup") {
    TEST_CASE("cleans doctest flags") {
        // Given
        const int argc { 4 };
        const char *input_args[argc] { R"(./build/nbpp-exe)", R"(--dt-tc="some test","some other test")", R"(add)", R"("some name")" };
        const std::string expected { R"(./build/nbpp-exe add "some name")" };

        // When
        const auto actual { clean_args(argc, input_args) };

        // Then
        CHECK(expected == actual);
    }

    TEST_CASE("returns false if string does NOT have prefix") {
        // Given
        const std::string prefix { "--dt-" };
        const char input[]  { "-flag" };

        // When
        const bool result { has_prefix(prefix, input) };

        // Then
        CHECK_FALSE(result);
    }

    TEST_CASE("returns true if string has prefix") {
        // Given
        const std::string prefix { "--dt-" };
        const char input[]  { "--dt-flag" };

        // When
        const bool result { has_prefix(prefix, input) };

        // Then
        CHECK(result);
    }
}
