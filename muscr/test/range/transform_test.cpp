#include "../catch.hpp"

#include "muscr/include/range/transform.h"


TEST_CASE("to_string", "[range]")
{
    using muscr::range::pitch_class;
    using muscr::range::to_string;

    REQUIRE(to_string(pitch_class{ 0 }) == "C");
    REQUIRE(to_string(pitch_class{ 1 }) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, true) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, false) == "C#");
}

TEST_CASE("string to_pitch_class", "[range]")
{
    using muscr::range::to_pitch_class;

    REQUIRE(0 == to_pitch_class("C"));
    REQUIRE(1 == to_pitch_class("C#"));
    REQUIRE(1 == to_pitch_class("Db"));
}

TEST_CASE("midi note number to pitch class", "[range]")
{
    using muscr::range::midi_note_number;
    using muscr::range::pitch_class;
    using muscr::range::to_pitch_class;

    REQUIRE(0 == to_pitch_class(midi_note_number{ 0 }));
    REQUIRE(7 == to_pitch_class(midi_note_number{ 127 }));
    REQUIRE(0 == to_pitch_class(midi_note_number{ 60 }));
}

TEST_CASE("pitch to midi note number", "[range]")
{
    using muscr::range::pitch;
    using muscr::range::pitch_class;

    REQUIRE(60 == to_midi_note_number(pitch<>{ 0 }));
    REQUIRE(60 == to_midi_note_number(pitch<>{ "C" }));
    REQUIRE(60 == to_midi_note_number(pitch<>{ "C", 3 }));
    REQUIRE(72 == to_midi_note_number(pitch<>{ "C", 4 }));

    REQUIRE(60 == to_midi_note_number(pitch<4>{ 0 }));
    REQUIRE(60 == to_midi_note_number(pitch<4>{ "C" }));
    REQUIRE(60 == to_midi_note_number(pitch<4>{ "C", 4 }));
    REQUIRE(72 == to_midi_note_number(pitch<4>{ "C", 5 }));
}
