module;
#include <stdexcept>
#include <string>
#include <unordered_map>

export module model.ClientInfo;

import quickfix_logger;
import model.Date;

/*** Public declarations ***/

export namespace model {
    struct ClientInfo {
        /**
         * Data points needed to run nbpp.
         */
        enum class Datum {
            /// Last edit of the nbpp repository.
            LAST_EDIT
        };

        Datum datum { };
        std::string value { };

        static auto datum_from(std::string_view str)  -> Datum;
    };
}


/*** Private declarations ***/

/**
 * A map of the string representation of an `Datum` to the enum representation of
 * an `Datum`
 */
static const std::unordered_map<std::string_view, model::ClientInfo::Datum> DATUM_MAP {
    { "last-edit", model::ClientInfo::Datum::LAST_EDIT }
};


/*** Public implementations ***/

/**
 * Retrieves an option from a string.
 * If no option is mapped to the given string, the program will terminate.
 */
auto model::ClientInfo::datum_from(std::string_view str) -> Datum {
    // TODO: Make it fail silent.
    try {
        return DATUM_MAP.at(str);
    } catch (std::out_of_range const&) {
        qlog::error("Encountered unknown option: '{}'", str);
        std::terminate();
    }
}
