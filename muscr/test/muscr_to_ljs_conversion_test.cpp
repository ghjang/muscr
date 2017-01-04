#include "catch.hpp"

#include "muscr/include/parser/leadsheet_staff.h"
#include "muscr/include/muscr_to_ljs_conversion.h"


TEST_CASE("muscr to ljs conversion", "[muscr to ljs]")
{
    std::string staff{
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

    // parsing
    auto srcAttr = muscr::to_leadsheet_attr(staff);

    // conversion
    auto destAttr = muscr::to_ljs_song_data_attr(srcAttr);

    REQUIRE("A Sample Song" == destAttr.title_);
    REQUIRE("ghjang" == destAttr.composer_);
    REQUIRE("C Major" == destAttr.keySignature_);
    REQUIRE("4 / 4" == destAttr.time_);

    REQUIRE(destAttr.sections_.size() == 1);
    auto & s = destAttr.sections_.front();
    REQUIRE("A" == s.name_);
    REQUIRE(s.bars_.size() == 4);

    auto & b = s.bars_.back();
    REQUIRE(b.chords_.size() == 1);
    REQUIRE(b.notes_.size() == 2);

    auto & c = b.chords_.front();
    REQUIRE("F" == c.pitchClass_);
    REQUIRE("" == c.modifier_);
    REQUIRE("" == c.accessaryModifier_);
    REQUIRE(1 == c.beatPos_);

    auto & n = b.notes_.front();
    REQUIRE("A" == n.pitchClass_);
    REQUIRE(3 == n.octavePos_);
    REQUIRE("h" == n.duration_);

    // generation
    auto ljsStr = muscr::to_ljs_song_data_str(destAttr);
    
    //std::cout << ljsStr << '\n';
}

TEST_CASE("muscr to ljs conversion - 1", "[muscr to ljs]")
{
    std::string staff{
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

    auto ljsSongDataStr = muscr::generate_ljs_song_data_module_str(staff);

    std::cout << ljsSongDataStr << '\n';
}
