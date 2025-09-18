module;
#include <doctest/doctest.h>
#include <cstdlib>
#include <optional>
#include <ranges>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

export module store_parser;

import model.Setting;

/*** Public declarations ***/
export namespace store_parser {
    struct Pair {
        const std::string key { };
        const std::string value { };
        Pair(std::string in_key, std::string in_value) : key { std::move(in_key) }, value { std::move(in_value) } {}
    };

    struct ListPair {
        const std::string key { };
        const std::vector<std::string> values { };
        ListPair(std::string in_key, std::vector<std::string> in_values) : key { std::move(in_key) }, values { std::move(in_values) } {}
    };

    auto key_value_from(std::string_view input) -> Pair;
    auto key_value_list_from(std::string_view) -> ListPair;
    auto match_value(std::string_view value, std::string_view input) -> std::optional<std::string>;
}

/*** Private declarations ***/
constexpr char ENV_VAR_CONFIG_PATH[] { "NBPP_CONFIG_PATH" };
constexpr char DEFAULT_CONFIG_PATH[] { ".config/nbpp " };

struct Token {
    const std::string literal { };
    const std::string_view remainder { };
};

auto parse_plural_token(std::string_view input)   -> std::vector<std::string>;
auto parse_single_token(std::string_view input) -> Token;


/*** Public implementations ***/

/**
 * Check if there is a match in an input line. The input line, must be a well
 * formed entry. If there is no match, it returns an empty option.
 */
auto store_parser::match_value(std::string_view value, std::string_view input) -> std::optional<std::string> {
    Token key_token { parse_single_token(input) };
    auto input_iter { key_token.remainder.cbegin() };
    auto value_iter { value.cbegin() };
    while (value_iter != value.cend() and input_iter != key_token.remainder.cend()) {
        if (*input_iter == *value_iter) {
            ++input_iter;
            ++value_iter;
        } else {
            value_iter = value.cbegin();
            ++input_iter;
        }
    }

    if (value_iter == value.cend()) {
        return key_token.literal;
    } else {
        return std::nullopt;
    }
}

/***
 * Retrieves a key-value pair from an input `std::string_view`.
 * NOTE: This function does not make any guarantees about the contents of the
 * key-value pair.
 */
auto store_parser::key_value_from(std::string_view input) -> Pair {
    Token key   { parse_single_token(input) };
    Token value { parse_single_token(key.remainder) };
    return { key.literal, value.literal };
}

/***
 * Retrieve a key-value-list pair from an input `std::stringview`.
 * NOTE: This function does not make any guarantees about the contents of the
 * key-value=list pair.
 */
auto store_parser::key_value_list_from(std::string_view input) -> ListPair {
    Token key_token { parse_single_token(input) };
    auto values     { parse_plural_token(key_token.remainder) };
    return { key_token.literal, values };
}

/*** Private implementations ***/

// TODO: I do not know if this function, should rest in this module
auto get_config_path() -> std::string {
    if (const char* c_str = getenv(ENV_VAR_CONFIG_PATH); c_str) {
        return { c_str };
    } else {
        std::string config_path { };
        config_path.append(getenv("HOME"));
        config_path.append(DEFAULT_CONFIG_PATH);
        return config_path;
    }
}

/**
 * Parses a plural token. 
 * NOTE: For a plural token to be well formed it must use "\\," as the value
 * delimiter.
 */
auto parse_plural_token(std::string_view input) -> std::vector<std::string> {
    constexpr std::string_view delimiter { "\\," };
    return input
        | std::views::split(delimiter)
        | std::ranges::to<std::vector<std::string>>();
}

/**
 * Parses a single token and returns a `Token`
 * NOTE: This method should NOT be used with list based tokens.
 */
auto parse_single_token(std::string_view input) -> Token {
    bool escaped { false };
    std::ostringstream stream { std::ios_base::ate };
    auto iter { input.cbegin() };
    while (iter != input.cend()) {
        if (escaped) {
            escaped = false;
            if (*iter != ':') {
                stream << '\\';
            }
            stream << *iter;
        } else {
            switch (*iter) {
                case ':' : {
                               if (++iter == input.cend()) {
                                   return { stream.str() };
                               } else {
                                   return { stream.str(), iter };
                               }
                           }
                case '\\': {
                               escaped = true;
                               break;
                           }
                default  : {
                               stream << *iter;
                               break;
                           }
            }
        }

        ++iter;
    }
    return { stream.str() };
}

/*** Tests ***/
TEST_SUITE("store-parser-tests") {
using namespace store_parser;

    TEST_CASE("respects environment variable 'NBPP_CONFIG_PATH'") {
        // Given
        bool env_config_path_predefined = false;
        const std::string expected { "/custom/path/to/config" };
        if (auto env_config_path_value = getenv(ENV_VAR_CONFIG_PATH); env_config_path_value) {
            env_config_path_predefined = true;
        } else {
            setenv(ENV_VAR_CONFIG_PATH, expected.c_str(), 1);
        }

        // When
        const std::string actual { get_config_path() };

        // Then
        CHECK(expected == actual);

        // Tear down
        if (!env_config_path_predefined) {
            unsetenv(ENV_VAR_CONFIG_PATH);
        }
    }

    TEST_CASE("Can parse well formed key-value pair") {
        // Given
        const std::string input { "last-edit:May 27 19\\:46" };
        const Pair expected { "last-edit", "May 27 19:46" };

        // When
        const auto actual { key_value_from(input) };

        // Then
        CHECK(expected.key  == actual.key);
        CHECK(expected.value == actual.value);
    }

    TEST_CASE("Can parse well formed key-value-list pair") {
        // Given
        const std::string input { "some-key:value0\\,value1\\,value2" };
        const ListPair expected {
            "some-key",
            { "value0", "value1", "value2" }
        };

        // When
        const auto actual { key_value_list_from(input) };

        // Then
        CHECK(expected.key == actual.key);

        REQUIRE(expected.values.size() == actual.values.size());
        CHECK(expected.values.at(0) == actual.values.at(0));
        CHECK(expected.values.at(1) == actual.values.at(1));
        CHECK(expected.values.at(2) == actual.values.at(2));

    }

    TEST_CASE("Single token parsing") {
        SUBCASE("Can parse well formed singular token") {
            // Given
            const std::string input { "default-editor:vim" };
            const Token expected { "default-editor", "vim" };

            // When
            const Token actual { parse_single_token(input) };

            // Then
            CHECK(expected.literal   == actual.literal);
            CHECK(expected.remainder == actual.remainder);
        }

        SUBCASE("Can parse date, with escaped colon") {
            // Given
            const std::string input { "May 27 19\\:46" };
            const Token expected { "May 27 19:46" };

            // When
            const Token actual { parse_single_token(input) };

            // Then
            CHECK(expected.literal   == actual.literal);
            CHECK(expected.remainder == actual.remainder);
        }

        SUBCASE("Keeps backslash before escaped non-colon characters") {
            // Given
            const std::string input { R"(regex\s\{1, 3\})" };
            const Token expected { R"(regex\s\{1, 3\})" };

            // When
            const Token actual { parse_single_token(input) };

            // Then
            CHECK(expected.literal   == actual.literal);
            CHECK(expected.remainder == actual.remainder);
        }
    }

    TEST_CASE("Plural token parsing") {
        SUBCASE("can parse well formed plural token") {
            // Given
            const std::string input { "value0\\,value1\\,value2" };
            const std::vector<std::string> expected { "value0", "value1", "value2" };

            // When
            const auto actual { parse_plural_token(input) };

            // Then
            REQUIRE(expected.size() == actual.size());
            for (auto const& pair : std::views::zip(expected, actual))
                CHECK(std::get<0>(pair) == std::get<1>(pair));
        }
    }

    TEST_CASE("Can match value from input") {
        SUBCASE("Can match middle token") {
            // Given
            const std::string input_line  { "key0:value0\\,value 1\\,value2" };
            const std::string input_value { "value 1" };
            const std::string expected    { "key0" };

            // When
            const auto actual { store_parser::match_value(input_value, input_line) };

            // Then
            REQUIRE(actual.has_value());
            CHECK(expected == actual.value());
        }

        SUBCASE("Can match last token") {
            // Given
            const std::string input_line  { "key0:value0\\,value1\\,value2" };
            const std::string input_value { "value2" };
            const std::string expected    { "key0" };

            // When
            const auto actual { store_parser::match_value(input_value, input_line) };

            // Then
            REQUIRE(actual.has_value());
            CHECK(expected == actual.value());
        }

        SUBCASE("Return nullopt, if no match") {
            // Given
            const std::string input_line  { "key0:value0\\,value1\\,value2" };
            const std::string input_value { "other value" };
            const std::string expected    { "key0" };

            // When
            const auto actual { store_parser::match_value(input_value, input_line) };

            // Then
            CHECK_FALSE(actual.has_value());
        }
    }
}
