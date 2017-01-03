#include "catch.hpp"

#include <utility> // for std::rel_ops

#include "muscr/include/division_ratio.h"


TEST_CASE("division ratio comparison", "[division ratio]")
{
    using namespace std::rel_ops;
    using muscr::division_ratio;

    division_ratio r0(1, 4);
    division_ratio r1(1, 4);
    REQUIRE(r0 == r1);

    division_ratio r2(1, 8);
    REQUIRE(r2 < r1);

    // NOTE: need to enclose the comparison expressions with extra parenthesis
    //          to make it work with catch.hpp macros like 'REQUIRE'.
    REQUIRE((r2 <= r1));
    REQUIRE((r1 > r2));
    REQUIRE((r1 >= r2));
    REQUIRE((r1 != r2));

    division_ratio r3(1, 0);
    bool wasDivByZeroThrown = false;
    try {
        if (r2 < r3) { }
        REQUIRE(!"un-reachable");
    } catch (std::overflow_error const& /* e */) {
        wasDivByZeroThrown = true;
    }
    REQUIRE(wasDivByZeroThrown);
}
