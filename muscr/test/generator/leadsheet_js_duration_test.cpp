#include "../catch.hpp"

#include "muscr/include/generator/leadsheet_js_duration.h"


TEST_CASE("duration value", "[ljs]")
{
    using namespace muscr::generator::ljs;

    REQUIRE(ratio_to_str(whole_note) == "w");
    REQUIRE(ratio_to_str(half_note) == "h");
    REQUIRE(ratio_to_str(quater_note) == "q");
    REQUIRE(ratio_to_str(eighth_note) == "8");
    REQUIRE(ratio_to_str(sixteenth_note) == "16");
    
    REQUIRE(ratio_to_str(whole_rest) == "wr");
    REQUIRE(ratio_to_str(half_rest) == "hr");
    REQUIRE(ratio_to_str(quater_rest) == "qr");
    REQUIRE(ratio_to_str(eighth_rest) == "8r");
    REQUIRE(ratio_to_str(sixteenth_rest) == "16r");
}
