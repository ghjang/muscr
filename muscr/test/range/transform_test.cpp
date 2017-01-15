#include "../catch.hpp"

#include <type_traits>

#include "muscr/include/range/transform.h"


TEST_CASE("appending char to std::string", "[tip]")
{
    char n = 4;
    std::string s{ "C#" };
    s.append({ static_cast<char>('0' + n) });
    REQUIRE(s == "C#4");

    static_assert(std::is_same<int, decltype('0' + n)>());
}

TEST_CASE("pitch class to string", "[range]")
{
    using muscr::range::pitch_class;

    // NOTE: to_string will be found by ADL.
    REQUIRE(to_string(pitch_class{ 0 }) == "C");
    REQUIRE(to_string(pitch_class{ 1 }) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, true) == "Db");
    REQUIRE(to_string(pitch_class{ 1 }, false) == "C#");
}

TEST_CASE("string to pitch class", "[range]")
{
    using muscr::range::to_pitch_class;

    REQUIRE(0 == to_pitch_class("C"));
    REQUIRE(1 == to_pitch_class("C#"));
    REQUIRE(1 == to_pitch_class("Db"));
}

TEST_CASE("midi note number to pitch class", "[range]")
{
    using muscr::range::midi_note_number;

    REQUIRE(0 == to_pitch_class(midi_note_number{ 0 }));
    REQUIRE(7 == to_pitch_class(midi_note_number{ 127 }));
    REQUIRE(0 == to_pitch_class(midi_note_number{ 60 }));
}

TEST_CASE("pitch to midi note number", "[range]")
{
    using muscr::range::pitch;

    REQUIRE(60 == to_midi_note_number(pitch<>{ 0 }));
    REQUIRE(60 == to_midi_note_number(pitch<>{ "C" }));
    REQUIRE(60 == to_midi_note_number(pitch<>{ "C", 3 }));
    REQUIRE(72 == to_midi_note_number(pitch<>{ "C", 4 }));

    REQUIRE(60 == to_midi_note_number(pitch<4>{ 0 }));
    REQUIRE(60 == to_midi_note_number(pitch<4>{ "C" }));
    REQUIRE(60 == to_midi_note_number(pitch<4>{ "C", 4 }));
    REQUIRE(72 == to_midi_note_number(pitch<4>{ "C", 5 }));
}

TEST_CASE("midi note number to pitch", "[range]")
{
    using muscr::range::midi_note_number;
    using muscr::range::pitch;

    // NOTE: function template will not be found by ADL.
    using muscr::range::to_pitch;

    auto p = to_pitch<>(midi_note_number{ 60 });
    REQUIRE(p.pc_ == 0);
    REQUIRE(p.octavePos_ == 3);

    auto p1 = to_pitch<3>(midi_note_number{ 60 });
    REQUIRE(p1.pc_ == 0);
    REQUIRE(p1.octavePos_ == 3);
    
    auto p2 = to_pitch<4>(midi_note_number{ 60 });
    REQUIRE(p2.pc_ == 0);
    REQUIRE(p2.octavePos_ == 4);

    auto p3 = to_pitch<>(midi_note_number{ 72 });
    REQUIRE(p3.pc_ == 0);
    REQUIRE(p3.octavePos_ == 4);

    auto p4 = to_pitch<4>(midi_note_number{ 72 });
    REQUIRE(p4.pc_ == 0);
    REQUIRE(p4.octavePos_ == 5);

    auto p5 = to_pitch<4>(midi_note_number{ 81 });
    REQUIRE(p5.pc_ == 9);
    REQUIRE(p5.octavePos_ == 5);
}

TEST_CASE("pitch to string", "[range]")
{
    using muscr::range::pitch;
    using muscr::range::to_string;

    REQUIRE("C3" == to_string(pitch<>{ 0 }));
    REQUIRE("Db3" == to_string(pitch<>{ 1 }));
    REQUIRE("C#3" == to_string(pitch<>{ 1 }, false));
    REQUIRE("C4" == to_string(pitch<4>{ 0 }));
    REQUIRE("Db4" == to_string(pitch<4>{ 1 }));
    REQUIRE("C#4" == to_string(pitch<4>{ 1 }, false));
    
    REQUIRE("C3" == to_string(pitch<>{ "C" }));
    REQUIRE("Db3" == to_string(pitch<>{ "C#" }));
    REQUIRE("C#3" == to_string(pitch<>{ "C#" }, false));
    REQUIRE("C4" == to_string(pitch<>{ "C", 4 }));
    REQUIRE("Db4" == to_string(pitch<>{ "C#", 4 }));
    REQUIRE("C#4" == to_string(pitch<>{ "C#", 4 }, false));
}

TEST_CASE("string to pitch", "[range]")
{
    // NOTE: This can't be done with the current pitch class template.
    //        pitch class template's MidCOctaveNo non-type template parameter value
    //        can't be generated at runtime.  
}
