cmake_minimum_required(VERSION 3.24)
project(MiBar)

# Change between Debug and Release -> cmake . -DCMAKE_BUILD_TYPE=Debug && make && ./bin/MiBar
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC src/*.cpp)

# Find Lua
find_package(Lua REQUIRED)

include_directories(${LUA_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME}
    xcb
    xcb-util
    xcb-icccm
    xcb-randr
    xcb-image
    xcb-shm
    xcb-shape
    ${LUA_LIBRARIES}
)
