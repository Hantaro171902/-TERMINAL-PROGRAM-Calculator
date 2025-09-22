cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalCalculator VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/cursor_input.cpp
    src/color.cpp
    src/calculator.cpp
)

set(HEADERS
    src/headers/utils.hpp
    src/headers/cursor_input.hpp
    src/headers/color.hpp
    src/headers/calculator.hpp
)

# Exe
add_executable(Calculator ${SOURCES} ${HEADERS})

target_include_directories(Calculator PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
