#include "../catch.hpp"

#include <vector>

#include "muscr/include/range/midi_note.h"


TEST_CASE("midi note range", "[range]")
{
    using namespace ranges;
    using muscr::range::midi_note_number_range_view;
    using number_t = midi_note_number_range_view::number_type;

    midi_note_number_range_view notes;
    std::vector<number_t> first10Notes = notes | view::take(10);
    REQUIRE(10 == first10Notes.size());
    REQUIRE(0 == first10Notes.front());
    REQUIRE(9 == first10Notes.back());

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
}
