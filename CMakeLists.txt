cmake_minimum_required(VERSION 4.0.0)
project(nbpp VERSION 0.0.1 LANGUAGES CXX)

# Setup
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(nbpp_compile_flags INTERFACE)
target_compile_features(nbpp_compile_flags INTERFACE cxx_std_23)
target_compile_options(nbpp_compile_flags INTERFACE
    "$<${gcc_like_cxx}:-Wall;-Wextra;-Wthread-safety;-Wshadow;-Wredundant-move;-Wredundant-parens;-Wmissing-designated-field-initializers;-Wconsumed>"
)


# Dependencies
find_package(doctest        REQUIRED)
find_package(spdlog  CONFIG REQUIRED)

# Modules

## Quickfix logger
add_library(quickfix_logger)
target_sources(quickfix_logger
    PUBLIC
    FILE_SET CXX_MODULES FILES
        src/logger.cpp
)
target_link_libraries(quickfix_logger
    PRIVATE spdlog::spdlog
    PUBLIC  nbpp_compile_flags
)

## utility functions 
add_library(utils)
target_sources(utils
    PUBLIC
    FILE_SET CXX_MODULES FILES
        src/util.cpp
)
target_link_libraries(utils
    PRIVATE doctest::doctest
    PUBLIC nbpp_compile_flags
)

## model
add_subdirectory(src/model)

## parsers
add_subdirectory(src/parser)

## Main module, responsible for running the application
add_library(nbpp-app)
target_sources(nbpp-app
    PUBLIC
    FILE_SET CXX_MODULES FILES
        src/nbpp.cpp
)
target_link_libraries(nbpp-app
    PRIVATE utils
    PRIVATE parser
)

# Executables
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ## Debug
    message(NOTICE "Building with debug configuration")
    add_executable(nbpp src/debug_main.cpp)

    target_compile_definitions(nbpp PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)
    target_compile_definitions(nbpp PRIVATE DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS)

    target_link_libraries(nbpp
        PRIVATE doctest::doctest
        PRIVATE nbpp_compile_flags
        PRIVATE nbpp-app
    )
else()
    ## Release
    message(NOTICE "Building with release configuration")
    add_executable(nbpp src/release_main.cpp)
    ### Adding global definition to disable doctest
    add_definitions(-DDOCTEST_CONFIG_DISABLE)
    target_compile_options(nbpp-exe INTERFACE
        "$<${gcc_like_cxx}:-Wall;-Wextra;>"
    )

    target_link_libraries(nbpp
        PRIVATE doctest::doctest
        PRIVATE nbpp-app
    )
endif()
