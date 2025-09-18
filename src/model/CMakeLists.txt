add_library(model)
target_sources(model
    PUBLIC
    FILE_SET CXX_MODULES FILES
        ClientInfo.cpp
        Date.cpp
        Entry.cpp
        Setting.cpp
)
target_link_libraries(model
    PRIVATE doctest::doctest
    PRIVATE quickfix_logger
    PUBLIC nbpp_compile_flags
)
