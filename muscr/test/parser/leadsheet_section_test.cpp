#include "../catch.hpp"

#include "muscr/include/parser/muscr_section.h"

#include "muscr/include/utility.h"


TEST_CASE("pitch class match", "[leadsheet section]")
{
    using muscr::pitch_class;
    using tools::test_phrase_parser;

    pitch_class<std::string::iterator> pc_;

    std::string pcs[] = {
        "C",  "D",  "E",  "F",  "G",  "A",  "B",
        "C#", "D#", "E#", "F#", "G#", "A#", "B#",
        "Cb", "Db", "Eb", "Fb", "Gb", "Ab", "Bb"
    };
    for (auto & s : pcs) {
        REQUIRE(test_phrase_parser(s, pc_));
    }

    std::string wrongPcs[] = {
        "Z", "c", "cb", "c#", "C#b", "Cb#"
    };

    for (auto & s : wrongPcs) {
        REQUIRE_FALSE(test_phrase_parser(s, pc_));
    }
}

TEST_CASE("triad chord match", "[leadsheet section]")
{
    using muscr::triad_chord;
    using tools::test_phrase_parser;

    triad_chord<std::string::iterator> triad_;
    
    std::string chords[] = {
        "C",   "D",   "E",   "F",   "G",   "A",   "B",
        "C#",  "D#",  "E#",  "F#",  "G#",  "A#",  "B#",
        "Cb",  "Db",  "Eb",  "Fb",  "Gb",  "Ab",  "Bb",
        "Cm",  "Dm",  "Em",  "Fm",  "Gm",  "Am",  "Bm",
        "C#m", "D#m", "E#m", "F#m", "G#m", "A#m", "B#m",
        "Cbm", "Dbm", "Ebm", "Fbm", "Gbm", "Abm", "Bbm"
    };

    for (auto & s : chords) {
        REQUIRE(test_phrase_parser(s, triad_));
    }

    std::string wrongChords[] = {
        "Z", "CM", "C#z", "c#m", "C#M", "CbM"
    };

    for (auto & s : wrongChords) {
        REQUIRE_FALSE(test_phrase_parser(s, triad_));
    }
}

TEST_CASE("seventh chord match", "[leadsheet section]")
{
    using muscr::seventh_chord;
    using tools::test_phrase_parser;

    seventh_chord<std::string::iterator> seventh_;
    
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
        REQUIRE(test_phrase_parser(s, seventh_));
    }

    std::string wrongChords[] = {
        "Z", "CM", "C#z", "c#m", "C#M", "CbM",
        "Z7", "CM6", "C#z7", "c#m7", "C#M8", "CbM9",
    };

    for (auto & s : wrongChords) {
        REQUIRE_FALSE(test_phrase_parser(s, seventh_));
    }
}

TEST_CASE("melody division match", "[leadsheet section]")
{
    using muscr::division;
    using tools::test_phrase_parser;

    std::string divs[] = {
        "C, D",
        "E, (B, G)",
        "A",
        "(C, D)",
        "(E, (B, G))",
        "(E, (B, (G, D)))",
        "(E, (B, (G, (D))))",
        "(E, F, (B, (G, (D))))",
        "(E, F, (B, (G, (D)))), A, C",
        "(A)"
    };
    for (auto & s : divs) {
        REQUIRE(test_phrase_parser(s, division<std::string::iterator>{}));
    }

    std::string wrongDivs[] = {
        "C,, D",
        "E, B, G)",
        "A,",
        ",A",
        "((C, D)",
        "(E, )B, G))",
        ",(A)",
        "Am7",
        "G7",
        "Am7, G7"
    };
    for (auto & s : wrongDivs) {
        REQUIRE_FALSE(test_phrase_parser(s, division<std::string::iterator>{}));
    }
}

TEST_CASE("chord division match", "[leadsheet section]")
{
    using muscr::chord_division;
    using tools::test_phrase_parser;

    std::string divs[] = {
        "C, D",
        "E, (B, G)",
        "A",
        "(C, D)",
        "(E, (B, G))",
        "(E, (B, (G, D)))",
        "(E, (B, (G, (D))))",
        "(E, F, (B, (G, (D))))",
        "(E, F, (B, (G, (D)))), A, C",
        "(A)",

        "Dm",
        "Dm, Am, A, (G, Gm), F",
        "D7",
        "C, D7",
        "E, (Bm7, G)",
        "A",
        "(C, D)",
        "(E, (BM7, G))",
        "(E, (Bm, (G, D)))",
        "(E, (B, (G, (D))))",
        "(E, Fm, (B, (G7, (Dm))))",
        "(E, F, (B, (G, (D)))), A, Cm7",
        "(E, Fm, (BM7, (G, (DM7)))), A, Cm7",
        "(Am7)"
    };
    for (auto & s : divs) {
        REQUIRE(test_phrase_parser(s, chord_division<std::string::iterator>{}));
    }

    std::string wrongDivs[] = {
        "C,, D",
        "E, B, G)",
        "A,",
        ",A",
        "((C, D)",
        "(E, )B, G))",
        ",(A)"

        "C,, D",
        "E, BM7, G)",
        "A,",
        ",A",
        "((C7, D)",
        "(E, )B, G))",
        ",(A)"
    };
    for (auto & s : wrongDivs) {
        REQUIRE_FALSE(test_phrase_parser(s, chord_division<std::string::iterator>{}));
    }
}

TEST_CASE("melody section line match", "[leadsheet section]")
{
    using muscr::division;
    using tools::test_phrase_parser;

    division<std::string::iterator> div;

    std::string lines[] = {
        "C, D | E, (B, G) | A   | A, C",
    };
    for (auto & s : lines) {
        REQUIRE(test_phrase_parser(s, div % '|'));
    }

    std::string wrongLines[] = {
        "Z, D | E, (B, G) | A   | A, C",
        "C, D = E, (B, G) | A   | A, C",
        "C, D = E, (B, G) | A   | A, C |",
        "C, D > E, (B, G) | A   | A, C",
    };
    for (auto & s : wrongLines) {
        REQUIRE_FALSE(test_phrase_parser(s, div % '|'));
    }
}


struct str_cat : boost::static_visitor<>
{
    void operator () (std::string const& s)
    {
        str_ += s;
    }

    template <typename T>
    void operator () (std::vector<T> const& v)
    {
        for (auto & e : v) {
            boost::apply_visitor(*this, e);
        }
    }

    std::string str_;
};

TEST_CASE("melody section line attribute", "[leadsheet section]")
{
    using muscr::division;
    using muscr::division_attr;
    using tools::test_phrase_parser_attr;

    division<std::string::iterator> div;
    std::vector<division_attr> attr;

    //tools::display_attribute_of_parser(div % '|');

    std::string lines[] = {
        "C, D | E, (B, G) | A   | A, C",
    };
    for (auto & s : lines) {
        attr.clear();
        REQUIRE(test_phrase_parser_attr(s, div % '|', attr));
    }

    std::string combinedStrs[4] = {
        "CD", "EBG", "A", "AC"
    };
    for (std::size_t i = 0; i < 4; ++i) {
        str_cat str_cat_;
        for (auto & e : attr[i]) {
            boost::apply_visitor(str_cat_, e);
        }
        REQUIRE(str_cat_.str_ == combinedStrs[i]);
    }

    std::string wrongLines[] = {
        "Z, D | E, (B, G) | A   | A, C",
        "C, D = E, (B, G) | A   | A, C",
        "C, D = E, (B, G) | A   | A, C |",
        "C, D > E, (B, G) | A   | A, C",
    };
    for (auto & s : wrongLines) {
        attr.clear();
        REQUIRE_FALSE(test_phrase_parser_attr(s, div % '|', attr));
    }
}

TEST_CASE("chord section line match", "[leadsheet section]")
{
    using muscr::chord_division;
    using tools::test_phrase_parser;

    chord_division<std::string::iterator> div;

    std::string lines[] = {
        "C    | Em        | Am  | F",
        "C    | Em7        | Am  | FM7",
        "C    | Em, (G7, F)        | Am, CM7  | F",
    };
    for (auto & s : lines) {
        REQUIRE(test_phrase_parser(s, div % '|'));
    }

    std::string wrongLines[] = {
        "|C    | Em        | Am  | F",
        "C    | Em        | Am  | F|",
        "Z    | Em        | Am  | F",
        "C    = Em        | Am  | F",
        "C    = Em        | Am  > F",
    };
    for (auto & s : wrongLines) {
        REQUIRE_FALSE(test_phrase_parser(s, div % '|'));
    }
}

TEST_CASE("chord section line match attribute", "[leadsheet section]")
{
    using muscr::chord_division;
    using muscr::division_attr;
    using tools::test_phrase_parser_attr;

    chord_division<std::string::iterator> div;
    std::vector<division_attr> attr;

    std::string lines[] = {
        "C    | Em        | Am  | F",
        "C    | Em7        | Am  | FM7",
        "C    | Em, (G7, F)        | Am, CM7  | F",
    };
    for (auto & s : lines) {
        attr.clear();
        REQUIRE(test_phrase_parser_attr(s, div % '|', attr));
    }

    REQUIRE(attr.size() == 4);
    std::string combinedStrs[4] = {
        "C", "EmG7F", "AmCM7", "F"  
    };
    for (std::size_t i = 0; i < 4; ++i) {
        str_cat str_cat_;
        for (auto & e : attr[i]) {
            boost::apply_visitor(str_cat_, e);
        }
        REQUIRE(str_cat_.str_ == combinedStrs[i]);
    }

    std::string wrongLines[] = {
        "|C    | Em        | Am  | F",
        "C    | Em        | Am  | F|",
        "Z    | Em        | Am  | F",
        "C    = Em        | Am  | F",
        "C    = Em        | Am  > F",
    };
    for (auto & s : wrongLines) {
        REQUIRE_FALSE(test_phrase_parser_attr(s, div % '|', attr));
    }
}

TEST_CASE("leadsheet section match", "[leadsheet section]")
{
    using muscr::leadsheet_section;
    using tools::test_phrase_parser;

    leadsheet_section<std::string::iterator> section;

    std::string sections[] = {
        R"(
A :=
{
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}
        )",
    };
    for (auto & s : sections) {
        REQUIRE(test_phrase_parser(s, section));
    }

    std::string wrongSections[] = {
        R"(
A :=
<
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}
        )",        
        R"(
A =
<
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}
        )",        
        R"(
A =
<
    C, D | E, (B, G) | A    A, C
    C    | Em        | Am  | F
}
        )",        
    };
    for (auto & s : wrongSections) {
        REQUIRE_FALSE(test_phrase_parser(s, section));
    }
}
