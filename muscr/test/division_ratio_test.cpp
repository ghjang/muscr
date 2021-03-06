#include "catch.hpp"

#include <utility> // for std::rel_ops

#include "muscr/include/division_ratio.h"


TEST_CASE("division ratio comparison", "[division ratio]")
{
    using namespace std::rel_ops;
    using muscr::division_ratio::ratio;

    ratio<> r0(1, 4);
    ratio<> r1(1, 4);
    REQUIRE(r0 == r1);

    ratio<> r2(1, 8);
    REQUIRE(r2 < r1);

    // NOTE: need to enclose the comparison expressions with extra parenthesis
    //          to make it work with catch.hpp macros like 'REQUIRE'.
    REQUIRE((r2 <= r1));
    REQUIRE((r1 > r2));
    REQUIRE((r1 >= r2));
    REQUIRE((r1 != r2));

    ratio<> r3(1, 0);
    bool wasDivByZeroThrown = false;
    try {
        if (r2 < r3) { }
        REQUIRE(!"un-reachable");
    } catch (std::overflow_error const& /* e */) {
        wasDivByZeroThrown = true;
    }
    REQUIRE(wasDivByZeroThrown);
}

TEST_CASE("named ratio comparison", "[division ratio]")
{
    using namespace muscr::division_ratio;

    REQUIRE(half_note < whole_note);
    REQUIRE(quater_note < half_note);
    REQUIRE(eighth_note < quater_note);
    REQUIRE(sixteenth_note < eighth_note);

    REQUIRE(half_rest < whole_rest);
    REQUIRE(quater_rest < half_rest);
    REQUIRE(eighth_rest < quater_rest);
    REQUIRE(sixteenth_rest < eighth_rest);
}

TEST_CASE("ratio multiplication", "[division ratio]")
{
    using namespace muscr::division_ratio;

    REQUIRE(ratio<>{ 4 } == ratio<>{ 1 } * ratio<>{ 4 });
    REQUIRE(ratio<>{ 2 } == ratio<>{ 1 } * ratio<>{ 2 });
    REQUIRE(ratio<>{ 3 } == ratio<>{ 1 } * ratio<>{ 3 });
    REQUIRE((ratio<>{ 6, 2 } == ratio<>{ 1, 2 } * ratio<>{ 6 }));

    REQUIRE((ratio<>{ 6, 2 } == eighth_note * ratio<>{ 6 }));

    auto r = ratio<>{ 1, 2 } * ratio<>{ 6 };
    REQUIRE(ratio<>{ 3 } == r.to_lowest_term());
}

TEST_CASE("ratio division", "[division ratio]")
{
    using namespace muscr::division_ratio;

    REQUIRE(2 % 3 == 2);

    auto r = ratio<>{ 1, 2 } * ratio<>{ 6 };
    
    auto r1 = r.to_lowest_term();
    REQUIRE(ratio<>{ 3 } == r1);

    auto r2 = r1 / ratio<>{ 3 };
    REQUIRE((ratio<>{ 3, 3 } == r2));
    REQUIRE(ratio<>{ 1 } == r2.to_lowest_term());

    auto r4 = ratio<>{ 1 } * ratio<> { 4 };
    REQUIRE((ratio<>{ 4, 3 } == r4 / ratio<>{ 3 }));
}
