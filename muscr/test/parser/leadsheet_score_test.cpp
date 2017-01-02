#include "../catch.hpp"

#include "muscr/include/parser/muscr_leadsheet_staff.h"

#include "muscr/include/utility.h"


TEST_CASE("leadsheet score match", "[leadsheet staff]")
{
    using tools::test_phrase_parser;

    std::string scores[] = {
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

    muscr::leadsheet_score<std::string::iterator> score_;

    for (auto & s : scores) {
        REQUIRE(test_phrase_parser(
                    s,
                    score_
                ));
    }

    std::string wrongScores[] = {
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

    for (auto & s : wrongScores) {
        REQUIRE_FALSE(test_phrase_parser(
                            s,
                            score_
                      ));
    }
}
