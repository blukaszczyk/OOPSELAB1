cmake_minimum_required(VERSION 3.14)
project(ChannelTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ChannelTest
    ChannelTest.cpp
    Channel.cpp
)

target_link_libraries(
    ChannelTest
    GTest::gtest_main
    gmock
)

include(GoogleTest)
gtest_discover_tests(ChannelTest)