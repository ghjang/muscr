cmake_minimum_required(VERSION 3.5.2)
project(muscr_parser_test)

set(BOOST_ROOT "../../../boost_1_62_0")
find_package(Boost 1.62.0)
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
        parser/parser_test.cpp
        parser/lex_test.cpp
        parser/qi_test.cpp
        parser/leadsheet_property_test.cpp
        parser/leadsheet_section_test.cpp
        parser/leadsheet_score_test.cpp)

add_executable(muscr_parser_test ${SOURCE_FILES})

