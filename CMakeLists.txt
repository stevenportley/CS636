cmake_minimum_required(VERSION 2.8.9)
project(CS636)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/googletest)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main spdlog::spdlog)
target_link_libraries(main gtest_main)

