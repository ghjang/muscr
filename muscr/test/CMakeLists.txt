cmake_minimum_required(VERSION 3.5.2)
project(muscr_test)

set(BOOST_ROOT "../../../boost_1_63_0")
find_package(Boost 1.63.0)
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
        etc/parser_test.cpp
        etc/lex_test.cpp
        etc/qi_test.cpp
        parser/leadsheet_property_test.cpp
        parser/leadsheet_section_test.cpp
        parser/leadsheet_staff_test.cpp
        generator/leadsheet_js_song_test.cpp
        muscr_to_ljs_conversion_test.cpp
        division_ratio_test.cpp)

add_executable(muscr_test ${SOURCE_FILES})
