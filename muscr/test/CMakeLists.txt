cmake_minimum_required(VERSION 3.6)
project(muscr_parser_test)

set(BOOST_ROOT "../../../boost_1_60_0")
find_package(Boost 1.60.0)
if(Boost_FOUND)
    include_directories(${BOOST_ROOT})
else()
    message(FATAL_ERROR "Could not find boost!")
endif()

set(MUSCR_ROOT "../..")
include_directories(${MUSCR_ROOT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(SOURCE_FILES
        main.cpp
        parser/parser_test.cpp)

add_executable(muscr_parser_test ${SOURCE_FILES})

