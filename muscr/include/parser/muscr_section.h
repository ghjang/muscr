#ifndef MUSCR_SECTION_H
#define MUSCR_SECTION_H


#include <cstdint>
#include <string>
#include <vector>

#include <boost/variant/recursive_variant.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/qi.hpp>


namespace muscr
{
    namespace qi = boost::spirit::qi;
    using qi::eps;
    using qi::lit;
    using qi::char_;
    using qi::string;
    using qi::alpha;
    using qi::alnum;
    using qi::eol;
    using qi::lexeme;

    template <typename Iterator>
    qi::rule<Iterator, std::string()> pitch_class{
        char_("CDEFGAB") >> -char_("#b")
    };

    template <typename Iterator>
    qi::rule<Iterator, std::string()> triad_chord{
        pitch_class<Iterator> >> -char_('m')
    };

    template <typename Iterator>
    qi::rule<Iterator, std::string()> seventh_chord{
        triad_chord<Iterator> >> (string("M7") | char_('7'))
    };

    template <typename Iterator>
    qi::rule<Iterator, std::string()> chord{
        pitch_class<Iterator>
            >> -char_('m')
            >> -(string("M7") | char_('7'))
    };

    using subdivision_attr = boost::make_recursive_variant<
                                    std::string,
                                    std::vector<boost::recursive_variant_>
                             >::type;

    using division_attr = std::vector<subdivision_attr>;

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct division
            : qi::grammar<
                    Iterator,
                    division_attr(),
                    SpaceType
              >
    {
        division() : division::base_type(div_)
        {
            pc_ %= pitch_class<Iterator>;

            subDiv_ %= pc_ | '(' >> div_ >> ')';

            div_ %= subDiv_ % ',';
        }

        qi::rule<Iterator, std::string()> pc_;
        qi::rule<Iterator, subdivision_attr(), SpaceType> subDiv_;
        qi::rule<
                Iterator,
                division_attr(),
                SpaceType
        > div_;
    };        

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct chord_division
            : qi::grammar<
                    Iterator,
                    division_attr(),
                    SpaceType
              >
    {
        chord_division() : chord_division::base_type(div_)
        {
            chord_ %= lexeme[
                            char_("CDEFGAB") >> -char_("#b")
                                >> -char_('m')
                                >> -(string("M7") | char_('7'))
                      ];

            subDiv_ %= chord_ | '(' >> div_ >> ')';

            div_ %= subDiv_ % ',';
        }

        qi::rule<Iterator, std::string(), SpaceType> chord_;
        qi::rule<Iterator, subdivision_attr(), SpaceType> subDiv_;
        qi::rule<
                Iterator,
                division_attr(),
                SpaceType
        > div_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct leadsheet_section
            : qi::grammar<
                    Iterator,
                    SpaceType
              >
    {
        leadsheet_section() : leadsheet_section::base_type(section_)
        {
            name_ = alpha >> *(alnum | char_("_'"));

            melody_line_ = division_ % '|';

            chord_line_ = chord_division_ % '|';

            section_ = name_ >> lit(":=") >> lit('{')
                                >> melody_line_
                                >> chord_line_
                             >> lit('}');
        }

        qi::rule<Iterator, SpaceType> name_;
        division<Iterator, SpaceType> division_;
        chord_division<Iterator, SpaceType> chord_division_;
        qi::rule<Iterator, SpaceType> melody_line_;
        qi::rule<Iterator, SpaceType> chord_line_;
        qi::rule<Iterator, SpaceType> section_;
    };
} // namespace muscr


#endif // MUSCR_SECTION_H
