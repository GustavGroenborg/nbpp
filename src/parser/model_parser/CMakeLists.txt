add_library(model_parser)
target_sources(model_parser
    PUBLIC
    FILE_SET CXX_MODULES FILES
        ClientInfoParser.cpp
        EntryParser.cpp
        SettingParser.cpp
)
target_link_libraries(model_parser
    PRIVATE doctest::doctest
    PRIVATE quickfix_logger
    PUBLIC nbpp_compile_flags
    PUBLIC model
    PUBLIC parser
)
