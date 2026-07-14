cmake_minimum_required(VERSION 3.25)
project(kitty_tabs)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/process.cpp
    src/kitty.cpp
    src/parser.cpp
    src/pipe.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(kitty_tabs PRIVATE nlohmann_json::nlohmann_json)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
