module;
#include <stdexcept>
#include <string>
#include <unordered_map>

export module model.Setting;

import quickfix_logger;

/*** Public declarations ***/
export namespace model {
    struct Setting {
        /**
         * Holds the alterable options of nbpp.
         */
        enum class Option {
            DEFAULT_EDITOR,
            NBPP_HOME
        };

        const Option option { };
        const std::string value { };

        static auto option_from(std::string_view str) -> Option;
    };
}


/*** Private declarations ***/

/**
 * A map of the string representation of an `Option` to the enum representation of
 * an `Option`.
 */
static const std::unordered_map<std::string_view, model::Setting::Option> OPTION_MAP {
    { "default-editor", model::Setting::Option::DEFAULT_EDITOR },
    { "nbpp-home", model::Setting::Option::NBPP_HOME },
};

/*** Public implementations ***/

/**
 * Retrieves an option from a string.
 * If no option is mapped to the given string, the program will terminate.
 */
auto model::Setting::option_from(std::string_view str) -> model::Setting::Option {
    try {
        return OPTION_MAP.at(str);
    } catch (std::out_of_range const&) {
        // TODO: The error should be propagated.
        qlog::error("Encountered unknown option: '{}'", str);
        std::terminate();
    }
}
