cmake_minimum_required(VERSION 3.20)
project(kitty_tabs C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(cJSON CONFIG REQUIRED)

set(SOURCES
    src/main.c
    src/process.c
    src/kitty.c
    src/parser.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(kitty_tabs PRIVATE cjson)

target_compile_options(${PROJECT_NAME} PRIVATE
    -std=c17
    -Wall
    -Wextra
    -Wpedantic
)
