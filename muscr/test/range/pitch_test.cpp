#include "../catch.hpp"

#include <vector>

#include "muscr/include/range/pitch.h"


TEST_CASE("pitch class", "[range]")
{
    using muscr::range::pitch_class;

    pitch_class pc{ 0 };

    REQUIRE(0 == pc);
    REQUIRE(0 == pc++);
    REQUIRE(1 == pc);

    pc = pitch_class{ 11 };
    REQUIRE(11 == pc);
    REQUIRE(0 == ++pc);
}

TEST_CASE("to_pitch_class", "[range]")
{
    using muscr::range::to_pitch_class;

    REQUIRE(0 == to_pitch_class("C"));
    REQUIRE(1 == to_pitch_class("C#"));
    REQUIRE(1 == to_pitch_class("Db"));
}

TEST_CASE("to_string", "[range]")
{
    using muscr::range::pitch_class;
    using muscr::range::to_string;

    REQUIRE(to_string(pitch_class{ 0 }) == "C");
    REQUIRE(to_string(pitch_class{ 1 }) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, true) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, false) == "C#");
}

TEST_CASE("pitch class range view", "[range]")
{
    using namespace ranges;
    using namespace muscr::range;

    pitch_class_view pcv;

    std::vector<pitch_class> pcs = pcv | view::take(7);
    REQUIRE(7 == pcs.size());
    REQUIRE(0 == pcs.front());
    REQUIRE(6 == pcs.back());

    std::vector<pitch_class> pcs1 = pcv | view::take(20);
    REQUIRE(20 == pcs1.size());
    REQUIRE(0 == pcs1.front());
    REQUIRE(7 == pcs1.back());
    REQUIRE("G" == to_string(pcs1.back()));
}
