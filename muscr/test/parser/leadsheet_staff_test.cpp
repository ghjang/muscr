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
@pitchRange: 4
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

    muscr::parser::leadsheet_staff<std::string::iterator> staff_;

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
@pitchRange: 4
@clef: G

@timeSignature: 4 / 4
@bpm: 100
)", // no sections

R"(
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 4
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

TEST_CASE("leadsheet staff attr", "[leadsheet staff]")
{
    using tools::test_phrase_parser_attr;

    std::string staffs[] = {
R"(
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 4
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

    muscr::parser::leadsheet_staff<std::string::iterator> staff_;
    muscr::parser::leadsheet_staff_attr attr;

    for (auto & s : staffs) {
        REQUIRE(test_phrase_parser_attr(
                    s,
                    staff_,
                    attr
                ));
    }

    REQUIRE("A Sample Song" == attr.properties_["title"]);
    REQUIRE("ghjang" == attr.properties_["author"]);
    REQUIRE("C Major" == attr.properties_["scale"]);
    REQUIRE("4" == attr.properties_["pitchRange"]);
    REQUIRE("G" == attr.properties_["clef"]);
    REQUIRE("4 / 4" == attr.properties_["timeSignature"]);
    REQUIRE("100" == attr.properties_["bpm"]);

    REQUIRE(1 == attr.sections_.size());
    REQUIRE("A" == attr.sections_[0].name_);
}
