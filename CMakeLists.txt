cmake_minimum_required(VERSION 3.20)

project(CustomVector
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# CustomVector Library
# =========================

add_library(custom_vector INTERFACE)

target_include_directories(custom_vector
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# =========================
# Tests
# =========================

add_executable(custom_vector_tests
    tests/test_custom_vector.cpp
)

target_link_libraries(custom_vector_tests
    PRIVATE
        custom_vector
)

enable_testing()

add_test(
    NAME custom_vector_tests
    COMMAND custom_vector_tests
)

# =========================
# Benchmark
# =========================

add_executable(custom_vector_benchmark
    benchmarks/benchmark_custom_vector.cpp
)

target_link_libraries(custom_vector_benchmark
    PRIVATE
        custom_vector
)

# =========================
# Example
# =========================

add_executable(custom_vector_example
    examples/basic_usage.cpp
)

target_link_libraries(custom_vector_example
    PRIVATE
        custom_vector
)