#include "../catch.hpp"

#include <vector>
#include <iostream>

#include "../../include/parser/muscr_property.h"
#include "../../include/utility.h"


/*
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100
    std::string str(read_from_file("/Users/gilhojang/GitHub/muscr/muscr/etc/sample_leadsheet_property.muscr"));
 */

TEST_CASE("simple match", "[leadsheet property]")
{
    using namespace boost::spirit;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;

    auto prop_matcher = [](auto begin, auto end) {
        bool r = qi::parse(
                        begin,
                        end,
                        *(blank | eol) >> '@' >> +alpha >> ':' >> +(char_ - eol)
                );
        return r;
    };

    std::string strArr[] = {
        "@title: A Sample Song\n",
        "@author: ghjang\n",
        "@scale: C Major\n",
        "@pitchRange: 3\n",
        "@clef: G\n",
        "@timeSignature: 4 / 4\n",
        "@bpm: 100\n",
        "@bpm: 100 \n\n",
        "       @bpm: 100\n",
        "\n \n@bpm: 100\n",
        R"(

                @bpm: 100

        )"
    };
    for (std::string & s : strArr) {
        bool matchResult = prop_matcher(s.begin(), s.end());
        if (!matchResult) {
            std::cerr << "failed to match: " << s << '\n';
        }
        REQUIRE(matchResult);
    }
}
