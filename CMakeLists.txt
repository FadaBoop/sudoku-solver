cmake_minimum_required(VERSION 3.16)
project(sudoku-solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sudoku-solver src/main.cpp)

# Ask for the new SFML 3 component(s).
# Graphics automatically brings in Window + System.
find_package(SFML 3 COMPONENTS Graphics REQUIRED)

# Link the imported SFML target(s)
target_link_libraries(sudoku-solver PRIVATE SFML::Graphics)