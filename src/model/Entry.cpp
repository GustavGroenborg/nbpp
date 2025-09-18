module;
#include <doctest/doctest.h>
#include <string>
#include <vector>

/*** Public declarations ***/
export module model.Entry;

export namespace model {
    struct Entry {
        std::string key { };
        std::vector<std::string> values { };

        [[nodiscard]] auto equals(const Entry& rhs) const -> bool {
            return key == rhs.key and values == rhs.values;
        }
    };
}

/*** Unit tests ***/
TEST_SUITE("entry-tests") {
    using namespace model;

    TEST_CASE("can compare") {
        // Given
        const Entry lhs { "key0", { "value0", "value1", "value2" } };
        const Entry rhs0 { "key0", { "value0", "value1", "value2" } };
        const Entry rhs1 { "key0", { "value0", "value1", "something else" } };

        // Then
        CHECK(lhs.equals(rhs0));
        CHECK_FALSE(lhs.equals(rhs1));
    }
}
