#include "../catch.hpp"

#include <vector>

#include "muscr/include/range/midi_note.h"


TEST_CASE("midi note number", "[range]")
{
    using muscr::range::midi_note_number;

    midi_note_number n{ 60 };
    REQUIRE(60 == n);
}

TEST_CASE("midi note number range view", "[range]")
{
    using namespace ranges;
    using muscr::range::midi_note_number;
    using muscr::range::midi_note_number_range_view;
    using number_t = midi_note_number_range_view::number_type;

    midi_note_number_range_view notes;
    std::vector<number_t> first10Notes = notes | view::take(10);
    REQUIRE(10 == first10Notes.size());
    REQUIRE(0 == first10Notes.front());
    REQUIRE(9 == first10Notes.back());

    std::vector<number_t> totalNotes = notes | view::take(256);
    REQUIRE(128 == totalNotes.size());
    REQUIRE(0 == totalNotes.front());
    REQUIRE(127 == totalNotes.back());

    midi_note_number_range_view notes1{ 60 }; // middle C
    std::vector<number_t> middleNotes = notes1 | view::take(12);
    REQUIRE(12 == middleNotes.size());
    REQUIRE(60 == middleNotes.front());
    REQUIRE(71 == middleNotes.back());

    midi_note_number_range_view notes2{ 60, 71 }; // from middle C to B
    std::vector<number_t> middleNotes1 = notes2 | view::take(24);
    REQUIRE(12 == middleNotes1.size());
    REQUIRE(60 == middleNotes1.front());
    REQUIRE(71 == middleNotes1.back()); // NOTE: it's still 71.

    midi_note_number_range_view notes3{ 60, 71 }; // from middle C to B
    std::vector<midi_note_number> middleNotes2 = notes2 | view::take(24); // used midi_note_number instead.
    REQUIRE(12 == middleNotes2.size());
    REQUIRE(60 == middleNotes2.front());
    REQUIRE(71 == middleNotes2.back()); // NOTE: it's still 71.
}
