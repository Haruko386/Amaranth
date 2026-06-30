cmake_minimum_required(VERSION 3.16)

project(amaranth
    VERSION 0.1.0
    DESCRIPTION "A lightweight C++ vector search engine for RAG experiments"
    LANGUAGES CXX
)

option(AMARANTH_BUILD_TESTS "Build Amaranth tests" ON)
option(AMARANTH_BUILD_EXAMPLES "Build Amaranth examples" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(amaranth
    src/distance.cpp
    src/flat_index.cpp
    src/collection.cpp
    src/database.cpp
    src/storage.cpp
)

target_include_directories(amaranth
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(amaranth PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(amaranth_cli apps/amaranth_cli/main.cpp)
target_link_libraries(amaranth_cli PRIVATE amaranth)

if (AMARANTH_BUILD_EXAMPLES)
    add_executable(insert_search_example examples/insert_search.cpp)
    target_link_libraries(insert_search_example PRIVATE amaranth)
endif()

if (AMARANTH_BUILD_TESTS)
    enable_testing()
    add_executable(test_flat_index tests/test_flat_index.cpp)
    target_link_libraries(test_flat_index PRIVATE amaranth)
    add_test(NAME test_flat_index COMMAND test_flat_index)
endif()
