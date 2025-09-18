module;
#include <doctest/doctest.h>
#include <sstream>
#include <string>

#include <iostream>

export module model.Date;
import quickfix_logger;

/*** Public declarations ***/
export namespace model {
    struct Date {
        unsigned short year       = 0;
        unsigned char  month      = 0;
        unsigned char  day        = 0;
        unsigned char  hour       = 0;
        unsigned char  minute     = 0;
        unsigned char  second     = 0;

        static auto from(std::string_view str) -> Date;

        [[nodiscard]] auto equals(const Date& rhs) const -> bool;
        [[nodiscard]] auto less_than(const Date& rhs) const -> bool;
        [[nodiscard]] auto greater_than(const Date& rhs) const -> bool;
        [[nodiscard]] auto to_64_bit_enconding() const -> unsigned long long;
    };
}

/*** Private declarations ***/
struct TimeToken {
    unsigned long literal { };
    std::string_view remainder { };
};

auto retrieve_time_token(std::string_view input) -> TimeToken;

/*** Public implementations ***/
auto model::Date::from(std::string_view str) -> Date {
    model::Date result { };
    auto time_token { retrieve_time_token(str) };
    result.year   = time_token.literal;

    time_token    = retrieve_time_token(time_token.remainder);
    result.month  = time_token.literal;

    time_token    = retrieve_time_token(time_token.remainder);
    result.day    = time_token.literal;

    time_token    = retrieve_time_token(time_token.remainder);
    result.hour   = time_token.literal;

    time_token    = retrieve_time_token(time_token.remainder);
    result.minute = time_token.literal;

    time_token    = retrieve_time_token(time_token.remainder);
    result.second = time_token.literal;
    
    return result;
}

auto model::Date::equals(const Date& rhs) const -> bool {
    return this->year == rhs.year
        && this->month == rhs.month
        && this->day == rhs.day
        && this->hour == rhs.hour
        && this->minute == rhs.minute
        && this->second == rhs.second;
}

auto model::Date::less_than(const Date& rhs) const -> bool {
    return this->to_64_bit_enconding() < rhs.to_64_bit_enconding();
}

auto model::Date::greater_than(const Date& rhs) const -> bool {
    return this->to_64_bit_enconding() > rhs.to_64_bit_enconding();
}

auto model::Date::to_64_bit_enconding() const -> unsigned long long {
    return ((unsigned long long) this->year  << 48)
        | ((unsigned long long) this->month  << 32)
        | ((unsigned long long) this->day    << 24)
        | ((unsigned long long) this->hour   << 16)
        | ((unsigned long long) this->minute << 8)
        | (unsigned long long) this->second;
}

/*** Private implementations ***/
auto retrieve_time_token(std::string_view input) -> TimeToken {
    std::ostringstream stream { std::ios_base::ate };
    auto iter { input.cbegin() };
    while (iter != input.cend()) {
        if (*iter != '-' and *iter != ':' and *iter != ' ') {
            stream << *iter;
            ++iter;
        } else {
            return { std::stoul(stream.str()), ++iter };
        }
    }

    return { std::stoul(stream.str()) };
}

TEST_SUITE("date-tests") {
    TEST_CASE("Can convert to 64 bit encoding") {
        // Given
        const model::Date input { 2025, 3, 2, 10, 29, 32 };
        const unsigned long long expected { 569'986'840'758'197'536 };

        // When
        const auto actual { input.to_64_bit_enconding() };

        // Then
        CHECK(expected == actual);
    }

    TEST_CASE("Can create from string") {
        // Given
        const std::string input { "2025-03-02 10:29:32.804601826" };
        const model::Date expected {
            2025, 3, 2, 10, 29, 32
        };

        // When
        const auto actual { model::Date::from(input) };

        // Then
        CHECK(expected.equals(actual));
    }

    TEST_CASE("Can compare greater than") {
        // Given
        const model::Date lhs { 2026, 3, 2, 10, 29, 32 };
        const model::Date rhs { 2025, 3, 2, 10, 29, 32 };

        // Then
        CHECK(lhs.greater_than(rhs));
    }

    TEST_CASE("Can compare less then") {
        // Given
        const model::Date lhs { 2025, 7, 3, 17, 56, 15 };
        const model::Date rhs { 2025, 7, 4, 19, 59, 35 };

        // Then
        CHECK(lhs.less_than(rhs));
    }
}
