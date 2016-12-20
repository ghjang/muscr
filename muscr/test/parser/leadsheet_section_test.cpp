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
