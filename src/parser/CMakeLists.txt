# TODO: I should consider making the store parser as a separate module and then
# combing the store_parser, and the model_parsers in a module named `parser`
add_library(store_parser)
target_sources(store_parser
    PUBLIC
    FILE_SET CXX_MODULES FILES
        store_parser.cpp
)
target_link_libraries(store_parser
    PRIVATE doctest::doctest
    PUBLIC nbpp_compile_flags
    PUBLIC model
)

add_subdirectory(model_parser)

add_library(parser)
target_sources(parser
    PUBLIC
    FILE_SET CXX_MODULES FILES
        parser.cpp
)
target_link_libraries(parser
    PUBLIC nbpp_compile_flags
    PUBLIC model
    PUBLIC store_parser
    PUBLIC model_parser
)
