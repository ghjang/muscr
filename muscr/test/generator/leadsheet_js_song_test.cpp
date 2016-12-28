#include "../catch.hpp"

#include <iterator>

#include "muscr/include/generator/leadsheet_js_song.h"

#include "muscr/include/utility.h"


TEST_CASE("chord part gen", "[ljs]")
{
    namespace karma = boost::spirit::karma;
    using karma::char_;
    using tools::test_generator_attr;

    REQUIRE(test_generator_attr("C", char_("CDEFGAB"), std::string("C")));
    REQUIRE(test_generator_attr("G", char_("CDEFGAB"), std::string("G")));

    REQUIRE(test_generator_attr("Ab", char_("CDEFGAB") << -char_("#b"), std::string("Ab")));

    REQUIRE(test_generator_attr(
                "p : 'A', ",
                "p : '" << char_("CDEFGAB") << -char_("#b") << "', ",
                std::string("A")
            ));
    REQUIRE(test_generator_attr(
                "p : 'A#', ",
                "p : '" << char_("CDEFGAB") << -char_("#b") << "', ",
                std::string("A#")
            ));
    REQUIRE(test_generator_attr(
                "p : 'Ab', ",
                "p : '" << char_("CDEFGAB") << -char_("#b") << "', ",
                std::string("Ab")
            ));

    // OK
    REQUIRE(test_generator_attr(
                "Abm",
                char_("CDEFGAB") << -char_("#b") << -char_('m'),
                std::string("Abm")
            ));

    // NOT OK. Huh?
    REQUIRE(!test_generator_attr(
                "Am",
                char_("CDEFGAB") << -char_("#b") << -char_('m'),
                std::string("Am")
            ));

    // NOTE: karma's alternate and the predicates consume attributes.
    //          And the alternate outputs the match result, but the predicates doesn't.
    REQUIRE(test_generator_attr(
                "Am",
                char_("CDEFGAB") << -char_("#b") << -char_('m'),
                std::string("Amm")
            ));
    REQUIRE(test_generator_attr(
                "Am",
                char_("CDEFGAB") << -char_("#b") << -char_('m'),
                std::string("AXm")
            ));    

            /*
    REQUIRE(test_generator_attr(
                "p : 'A', ch : 'm'",
                "p : '" << char_("CDEFGAB") << -char_("#b") << "', "
                    << "ch : '" << -char_('m') << '\'',
                std::string("Am")
            ));

    REQUIRE(test_generator_attr(
                "p : 'A', ch : 'm7'",
                "p : '" << char_("CDEFGAB") << -char_("#b") << "', "
                    << "ch : '" << -char_('m') << -(-char_('M') << char_('7')) << '\'',
                std::string("Am7")
            ));
            */
}

TEST_CASE("ljs gen", "[ljs]")
{
    namespace karma = boost::spirit::karma;
    using muscr::ljs::chord;
    using muscr::ljs::chord_attr;

    // NOTE: a output iterator is required.
    using sink_type = std::back_insert_iterator<std::string>;

    // NOTE : need to specify the output iterator for the template parameter.
    chord<sink_type> chord_;
    chord_attr attr{ "Am7", 1 };

    std::string s;
    sink_type sink(s);
    REQUIRE(karma::generate(sink, chord_, attr));
}

TEST_CASE("ljs chord gen", "[ljs]")
{
    using tools::test_generator_attr;
    using muscr::ljs::chord;
    using muscr::ljs::chord_attr;

    using sink_type = std::back_insert_iterator<std::string>;

    chord<sink_type> chord_;
    chord_attr attr{ "Am7", 3 };

    REQUIRE(test_generator_attr(
                "{ p : 'A', ch : 'm7', beat : 3 }",
                chord_, attr
            ));
}

TEST_CASE("leadsheet js song data", "[ljs]")
{

}
