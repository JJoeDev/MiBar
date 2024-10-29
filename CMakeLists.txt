cmake_minimum_required(VERSION 3.28)
project(MiBar)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Wconversion")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Cairo REQUIRED QUIET)

message(STATUS "Cairo Libraries: ${CAIRO_LIBRARIES}")

file(GLOB_RECURSE SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME}
  xcb
  xcb-util
  xcb-icccm
  xcb-randr
  ${CAIRO_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE "${CMAKE_SOURCE_DIR}/src/core/"
  PRIVATE "${CMAKE_SOURCE_DIR}/src/utils/"
  PRIVATE "${CAIRO_INCLUDE_DIRS}"
)
