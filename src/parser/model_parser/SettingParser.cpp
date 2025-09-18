module;
#include <doctest/doctest.h>
#include <string>

export module setting_parser;

import model.Setting;
import store_parser;

/*** Public declarations ***/
auto parse_setting(std::string_view input) -> model::Setting {
    auto pair { store_parser::key_value_from(input) };
    return {
        model::Setting::option_from(pair.key),
        pair.value
    };
}

/*** Private declarations ***/
constexpr char HEADING[] { "[settings]" };

/*** Public implementations ***/

/*** Private implementations ***/

TEST_SUITE("setting-parser") {
    TEST_CASE("Can parse well formed setting") {
        // Given
        const std::string input { "default-editor:vim" };
        const model::Setting expected {
            model::Setting::Option::DEFAULT_EDITOR,
            "vim"
        };

        // When
        const auto actual { parse_setting(input) };

        // Then
        CHECK(expected.option == actual.option);
        CHECK(expected.value  == actual.value);
    }
}
