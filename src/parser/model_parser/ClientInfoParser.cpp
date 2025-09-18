module;
#include <doctest/doctest.h>
#include <string>

export module client_info_parser;

import model.ClientInfo;
import store_parser;

/*** Public declarations ***/

/*** Private declarations ***/
constexpr char HEADING[] { "[Client info]" };

/**
 * Parses a line, from the client info file and returns it along with an info
 * type and the string value of the info.
 */
auto parse_client_info(std::string_view input) -> model::ClientInfo {
    auto pair { store_parser::key_value_from(input) };
    return {
        model::ClientInfo::datum_from(pair.key),
        pair.value
    };
}

TEST_SUITE("client-info-parser-tests") {
    TEST_CASE("Can parse last-edit info") {
        // Given
        const std::string input { R"(last-edit:569986840758197536)" };
        const model::ClientInfo expected {
            model::ClientInfo::Datum::LAST_EDIT,
            "569986840758197536"
        };

        // When
        const auto actual { parse_client_info(input) };

        // Then
        CHECK(expected.datum == actual.datum);
        CHECK(expected.value == actual.value);
    }
}
