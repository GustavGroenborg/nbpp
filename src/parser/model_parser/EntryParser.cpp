module;
#include <doctest/doctest.h>
#include <string>

export module entry_parser;

import model.Entry;
import store_parser;

/*** Public declarations ***/
auto parse_entry(std::string_view input) -> model::Entry {
    auto list_pair { store_parser::key_value_list_from(input) };
    return {
        list_pair.key,
        list_pair.values
    };
}

/*** Private declarations ***/
constexpr char HEADING[] { "[Entries]" };

/*** Public implementations ***/

/*** Private implementations ***/


TEST_SUITE("entry-parser-tests") {
    TEST_CASE("can parse well formed entry") {
        // Given
        const std::string input { R"(some-key:value0\,value1\,value2)" };
        const model::Entry expected { "some-key", { "value0", "value1", "value2" } };

        // When
        const auto actual { parse_entry(input) };

        // Then
        CHECK(expected.key == actual.key);

        REQUIRE(expected.values.size() == actual.values.size());
        CHECK(expected.values[0] == actual.values[0]);
        CHECK(expected.values[1] == actual.values[1]);
        CHECK(expected.values[2] == actual.values[2]);
    }
}
