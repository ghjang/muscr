#include "../catch.hpp"

#include <tuple>
#include <vector>
#include <iostream>

#include "muscr/include/parser/muscr_property.h"
#include "muscr/include/utility.h"


TEST_CASE("simple match", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using tools::test_parser;

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
        REQUIRE(test_parser(
                    s,
                    *(blank | eol) >> '@' >> +alpha >> ':' >> +(char_ - eol),
                    false
                ));
    }
}

TEST_CASE("simple match - 1", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using tools::test_parser;

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
    REQUIRE(test_parser(
                str,
                *(*(blank | eol) >> '@' >> +alpha >> ':' >> +(char_ - eol))
            ));
}

TEST_CASE("simple match - 2", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::omit;
    using tools::test_parser_attr;

    /*
    tools::display_attribute_of_parser(
        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol))
    );
    */

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

    std::vector<boost::fusion::vector<std::string, std::string>> propVec;

    REQUIRE(test_parser_attr(
                str,
                *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol)),
                propVec
            ));

    REQUIRE(propVec.size() == 7);
    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == " A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == " 100");
}

TEST_CASE("simple match - 3", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::omit;
    using tools::test_parser_attr;

    /*
    tools::display_attribute_of_parser(
        *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol))
    );
    */

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

    std::vector<muscr::parser::property> propVec;

    REQUIRE(test_parser_attr(
                str,
                *(omit[*(blank | eol)] >> '@' >> +alpha >> ':' >> +(char_ - eol)),
                propVec
            ));
    
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
    namespace qi = boost::spirit::qi;
    using qi::ascii::alpha;
    using qi::ascii::char_;
    using qi::ascii::blank;
    using qi::eol;
    using qi::lexeme;
    using tools::test_phrase_parser_attr;

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

    std::vector<muscr::parser::property> propVec;

    REQUIRE(test_phrase_parser_attr(
                str,
                *('@' >> +alpha >> ':' >> lexeme[+(char_ - eol)]),
                propVec
            ));
    
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
    namespace qi = boost::spirit::qi;
    using tools::test_phrase_parser_attr;
    using muscr::parser::prop_name_val_pair;

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

    prop_name_val_pair<std::string::iterator> prop_;
    std::vector<muscr::parser::property> propVec;

    REQUIRE(test_phrase_parser_attr(
                str,
                *prop_,
                propVec
            ));

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

TEST_CASE("rule match - 1", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using muscr::parser::global_properties;
    using tools::test_phrase_parser_attr;

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

    global_properties<std::string::iterator> properties;
    std::vector<muscr::parser::property> propVec;

    REQUIRE(test_phrase_parser_attr(
                str,
                properties,
                propVec
            ));

    REQUIRE(propVec.size() == 7);

    REQUIRE(boost::fusion::at_c<0>(propVec[0]) == "title");
    REQUIRE(boost::fusion::at_c<1>(propVec[0]) == "A Sample Song");
    REQUIRE(boost::fusion::at_c<0>(propVec[6]) == "bpm");
    REQUIRE(boost::fusion::at_c<1>(propVec[6]) == "100");

    REQUIRE(propVec[0].name_ == "title");
    REQUIRE(propVec[0].value_ == "A Sample Song");
    REQUIRE(propVec[6].name_ == "bpm");
    REQUIRE(propVec[6].value_ == "100");

    str = R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100

@timeSignature: 4 / 4)"; // duplicate property name

    propVec.clear();

    REQUIRE_FALSE(test_phrase_parser_attr(
                        str,
                        properties,
                        propVec
                  ));
}

TEST_CASE("rule match - 2", "[leadsheet property]")
{
    namespace qi = boost::spirit::qi;
    using muscr::parser::global_properties_map;
    using tools::test_phrase_parser_attr;

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

    global_properties_map<std::string::iterator> properties;
    std::map<std::string, std::string> propMap;

    REQUIRE(test_phrase_parser_attr(
                str,
                properties,
                propMap
            ));

    REQUIRE(propMap.size() == 7);

    REQUIRE("A Sample Song" == propMap["title"]);
    REQUIRE("100" == propMap["bpm"]);

    str = R"(

@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 3
@clef: G

@timeSignature: 4 / 4
@bpm: 100

@timeSignature: 4 / 4)"; // duplicate property name

    propMap.clear();

    REQUIRE_FALSE(test_phrase_parser_attr(
                        str,
                        properties,
                        propMap
                  ));
}
