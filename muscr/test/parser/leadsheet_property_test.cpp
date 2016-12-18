#include "../catch.hpp"

#include <tuple>
#include <vector>
#include <iostream>

#include "../../include/parser/muscr_property.h"
#include "../../include/utility.h"


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

TEST_CASE("simple match - 1", "[leadsheet property]")
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
                        *(*(blank | eol) >> '@' >> +alpha >> ':' >> +(char_ - eol))
                );
        return (r && begin == end);
    };

    std::string str{
        R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100)"     // NOTE: no newline at the last property.
    };
    REQUIRE(prop_matcher(str.begin(), str.end()));
}

TEST_CASE("simple match - 2", "[leadsheet property]")
{
    using namespace boost::spirit;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::omit;

    /*
    tools::display_attribute_of_parser(
        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol))
    );
    */

    std::vector<boost::fusion::vector<std::string, std::string>> propVec;

    auto prop_matcher = [&propVec](auto begin, auto end) {
        bool r = qi::parse(
                        begin,
                        end,
                        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol)),
                        propVec
                );
        return (r && begin == end);
    };

    std::string str{
        R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100)"     // NOTE: no newline at the last property.
    };
    REQUIRE(prop_matcher(str.begin(), str.end()));
    REQUIRE(propVec.size() == 7);
    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == " A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == " 100");
}

TEST_CASE("simple match - 3", "[leadsheet property]")
{
    using namespace boost::spirit;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::omit;

    /*
    tools::display_attribute_of_parser(
        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol))
    );
    */

    std::vector<muscr::property> propVec;

    auto prop_matcher = [&propVec](auto begin, auto end) {
        bool r = qi::parse(
                        begin,
                        end,
                        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol)),
                        propVec
                );
        return (r && begin == end);
    };

    std::string str{
        R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100)"     // NOTE: no newline at the last property.
    };
    REQUIRE(prop_matcher(str.begin(), str.end()));
    REQUIRE(propVec.size() == 7);

    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == " A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == " 100");

    REQUIRE(propVec[0].name_ == "title");
    REQUIRE(propVec[0].value_ == " A Sample Song");
    REQUIRE(propVec[6].name_ == "bpm");
    REQUIRE(propVec[6].value_ == " 100");
}

TEST_CASE("simple match - 4", "[leadsheet property]")
{
    using namespace boost::spirit;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::lexeme;

    std::vector<muscr::property> propVec;

    auto prop_matcher = [&propVec](auto begin, auto end) {
        bool r = qi::phrase_parse(
                        begin,
                        end,
                        *('@' >> +alpha >> ':' >> lexeme[+(char_ - eol)]),
                        qi::ascii::space,
                        propVec
                );
        return (r && begin == end);
    };

    std::string str{
        R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100)"     // NOTE: no newline at the last property.
    };
    REQUIRE(prop_matcher(str.begin(), str.end()));
    REQUIRE(propVec.size() == 7);

    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == "A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == "100");

    REQUIRE(propVec[0].name_ == "title");
    REQUIRE(propVec[0].value_ == "A Sample Song");
    REQUIRE(propVec[6].name_ == "bpm");
    REQUIRE(propVec[6].value_ == "100");
}

TEST_CASE("rule match", "[leadsheet property]")
{
    using namespace boost::spirit;

    std::vector<muscr::property> propVec;

    auto prop_matcher = [&propVec](auto begin, auto end) {
        auto prop_ = muscr::prop_str_val<std::string::iterator>;
        bool r = qi::phrase_parse(
                        begin,
                        end,
                        *prop_,
                        qi::ascii::space,
                        propVec
                );
        return (r && begin == end);
    };

    std::string str{
        R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100)"     // NOTE: no newline at the last property.
    };
    REQUIRE(prop_matcher(str.begin(), str.end()));
    REQUIRE(propVec.size() == 7);

    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == "A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == "100");

    REQUIRE(propVec[0].name_ == "title");
    REQUIRE(propVec[0].value_ == "A Sample Song");
    REQUIRE(propVec[6].name_ == "bpm");
    REQUIRE(propVec[6].value_ == "100");
}
