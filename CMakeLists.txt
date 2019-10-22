cmake_minimum_required(VERSION 3.7)

project(SDL2Test)

set(RESOURCE_DIR assets)
set(THIRD_PARTY_INCLUDE_DIRS third_party)
set(SRC_DIRS src)

set(CMAKE_CXX_STANDARD 11)
file(GLOB SRCS ${SRC_DIRS}/*.cpp ${SRC_DIRS}/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1") # enable SDL2 and WebGL 2
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${PROJECT_SOURCE_DIR}/${RESOURCE_DIR}@/${RESOURCE_DIR}") # bundle assets
    # make output html called index.html
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
else()
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    # copy assets to the build directory when the build is finished
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/${RESOURCE_DIR} $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${RESOURCE_DIR})
endif()

include_directories(${THIRD_PARTY_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLEW_LIBRARIES} SDL2 GL)
