#include "../catch.hpp"

#include "muscr/include/parser/leadsheet_staff.h"

#include "muscr/include/utility.h"


TEST_CASE("leadsheet staff match", "[leadsheet staff]")
{
    using tools::test_phrase_parser;

    std::string staffs[] = {
R"(
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100

A :=
{
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}
)"
    };

    muscr::leadsheet_staff<std::string::iterator> staff_;

    for (auto & s : staffs) {
        REQUIRE(test_phrase_parser(
                    s,
                    staff_
                ));
    }

    std::string wrongStaffs[] = {
R"(
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100
)", // no sections

R"(
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100

A ==
{
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}
)", // syntax error in the section
    };

    for (auto & s : wrongStaffs) {
        REQUIRE_FALSE(test_phrase_parser(
                            s,
                            staff_
                      ));
    }
}
