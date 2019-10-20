cmake_minimum_required(VERSION 3.7)

project(SDL2Test)

set (CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

set(THIRD_PARTY_INCLUDE_DIRS third_party)
set(SRC_DIRS src)

file(GLOB SRCS src/*.cpp src/*.h)

include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${THIRD_PARTY_INCLUDE_DIRS})

add_executable(SDL2Test ${SRCS})
target_link_libraries(SDL2Test ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} SDL2 SDL2main GL)
