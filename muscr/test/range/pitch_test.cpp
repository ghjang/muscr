#include "../catch.hpp"

#include <vector>

#include "muscr/include/range/pitch.h"
#include "muscr/include/range/transform.h"


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

// NOTE: catch.hpp has some operator overloadings on its own.
//          This makes conflicts with my overloadings... :(
//          pitch_class's implicit conversion to an integer seems to be the problem...
TEST_CASE("pitch class equal comparison", "[range]")
{
    using muscr::range::pitch_class;

    REQUIRE(equal(pitch_class{ 0 }, pitch_class{ 0 }));
    REQUIRE(!equal(pitch_class{ 0 }, pitch_class{ 2 }));
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

TEST_CASE("pitch equal comparison", "[range]")
{
    using muscr::range::pitch;

    REQUIRE(equal(pitch<>{ 0 }, pitch<>{ 0 }));
    REQUIRE(!equal(pitch<>{ 0 }, pitch<>{ 2 }));
    REQUIRE(!equal(pitch<>{ 0 }, pitch<4>{ 0 }));
    REQUIRE(!equal(pitch<>{ 0 }, pitch<>{ 0, 5 }));
}
