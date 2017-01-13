#include "../catch.hpp"

#include "muscr/include/range/transform.h"


TEST_CASE("to_string", "[range]")
{
    using muscr::range::pitch_class;
    using muscr::range::to_string;

    REQUIRE(to_string(pitch_class{ 0 }) == "C");
    REQUIRE(to_string(pitch_class{ 1 }) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, true) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, false) == "C#");
}

TEST_CASE("to_pitch_class", "[range]")
{
    using muscr::range::to_pitch_class;

    REQUIRE(0 == to_pitch_class("C"));
    REQUIRE(1 == to_pitch_class("C#"));
    REQUIRE(1 == to_pitch_class("Db"));
}

TEST_CASE("midi note number to pitch class", "[range]")
{

}
