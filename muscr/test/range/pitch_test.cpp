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
