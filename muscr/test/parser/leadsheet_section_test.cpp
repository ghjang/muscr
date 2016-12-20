#include "../catch.hpp"

#include "muscr/include/parser/muscr_section.h"


TEST_CASE("pitch class match", "[leadsheet section]")
{
    namespace qi = boost::spirit::qi;
    using muscr::pitch_class;


    auto pc_matcher = [](auto & s) {
        auto pc = pitch_class<std::string::iterator>;
        auto begin = s.begin();
        auto end = s.end();
        bool r = qi::parse(begin, end, pc);
        return (r && begin == end);
    };

    std::string pcs[] = {
        "C",  "D",  "E",  "F",  "G",  "A",  "B",
        "C#", "D#", "E#", "F#", "G#", "A#", "B#",
        "Cb", "Db", "Eb", "Fb", "Gb", "Ab", "Bb"
    };
    for (auto & s : pcs) {
        REQUIRE(pc_matcher(s));
    }

    std::string wrongPcs[] = {
        "Z", "c", "cb", "c#", "C#b", "Cb#"
    };

    for (auto & s : wrongPcs) {
        REQUIRE_FALSE(pc_matcher(s));
    }
}

TEST_CASE("triad chord match", "[leadsheet section]")
{
    namespace qi = boost::spirit::qi;
    using muscr::triad_chord;
    
    auto chord_matcher = [](auto & s) {
        auto triad = triad_chord<std::string::iterator>;
        auto begin = s.begin();
        auto end = s.end();
        bool r = qi::parse(begin, end, triad);
        return (r && begin == end);
    };

    std::string chords[] = {
        "C",   "D",   "E",   "F",   "G",   "A",   "B",
        "C#",  "D#",  "E#",  "F#",  "G#",  "A#",  "B#",
        "Cb",  "Db",  "Eb",  "Fb",  "Gb",  "Ab",  "Bb",
        "Cm",  "Dm",  "Em",  "Fm",  "Gm",  "Am",  "Bm",
        "C#m", "D#m", "E#m", "F#m", "G#m", "A#m", "B#m",
        "Cbm", "Dbm", "Ebm", "Fbm", "Gbm", "Abm", "Bbm"
    };

    for (auto & s : chords) {
        REQUIRE(chord_matcher(s));
    }

    std::string wrongChords[] = {
        "Z", "CM", "C#z", "c#m", "C#M", "CbM"
    };

    for (auto & s : wrongChords) {
        REQUIRE_FALSE(chord_matcher(s));
    }
}

TEST_CASE("seventh chord match", "[leadsheet section]")
{
    namespace qi = boost::spirit::qi;
    using muscr::seventh_chord;
    
    auto chord_matcher = [](auto & s) {
        auto seventh = seventh_chord<std::string::iterator>;
        auto begin = s.begin();
        auto end = s.end();
        bool r = qi::parse(begin, end, seventh);
        return (r && begin == end);
    };

    std::string chords[] = {
        "C7",   "D7",   "E7",   "F7",   "G7",   "A7",   "B7",
        "C#7",  "D#7",  "E#7",  "F#7",  "G#7",  "A#7",  "B#7",
        "Cb7",  "Db7",  "Eb7",  "Fb7",  "Gb7",  "Ab7",  "Bb7",
        "Cm7",  "Dm7",  "Em7",  "Fm7",  "Gm7",  "Am7",  "Bm7",
        "C#m7", "D#m7", "E#m7", "F#m7", "G#m7", "A#m7", "B#m7",
        "Cbm7", "Dbm7", "Ebm7", "Fbm7", "Gbm7", "Abm7", "Bbm7",

        "CM7",   "DM7",   "EM7",   "FM7",   "GM7",   "AM7",   "BM7",
        "C#M7",  "D#M7",  "E#M7",  "F#M7",  "G#M7",  "A#M7",  "B#M7",
        "CbM7",  "DbM7",  "EbM7",  "FbM7",  "GbM7",  "AbM7",  "BbM7",
        "CmM7",  "DmM7",  "EmM7",  "FmM7",  "GmM7",  "AmM7",  "BmM7",
        "C#mM7", "D#mM7", "E#mM7", "F#mM7", "G#mM7", "A#mM7", "B#mM7",
        "CbmM7", "DbmM7", "EbmM7", "FbmM7", "GbmM7", "AbmM7", "BbmM7"
    };

    for (auto & s : chords) {
        REQUIRE(chord_matcher(s));
    }

    std::string wrongChords[] = {
        "Z", "CM", "C#z", "c#m", "C#M", "CbM",
        "Z7", "CM6", "C#z7", "c#m7", "C#M8", "CbM9",
    };

    for (auto & s : wrongChords) {
        REQUIRE_FALSE(chord_matcher(s));
    }
}
