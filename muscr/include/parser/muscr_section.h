#ifndef MUSCR_SECTION_H
#define MUSCR_SECTION_H


#include <cstdint>
#include <string>
#include <vector>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/qi.hpp>

#include "muscr/include/parser/muscr_section_attr.h"


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

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct pitch_class : qi::grammar<Iterator, std::string(), SpaceType>
    {
        pitch_class() : pitch_class::base_type(pc_)
        {
            pc_ %= lexeme[char_("CDEFGAB") >> -char_("#b")];
        }

        qi::rule<Iterator, std::string(), SpaceType> pc_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct triad_chord : qi::grammar<Iterator, std::string(), SpaceType>
    {
        triad_chord() : triad_chord::base_type(triad_)
        {
            triad_ %= lexeme[
                            char_("CDEFGAB")
                                >> -char_("#b")
                                >> -char_('m')
                      ];
        }

        qi::rule<Iterator, std::string(), SpaceType> triad_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct seventh_chord : qi::grammar<Iterator, std::string(), SpaceType>
    {
        seventh_chord() : seventh_chord::base_type(seventh_)
        {
            seventh_ %= lexeme[
                            char_("CDEFGAB")
                                >> -char_("#b")
                                >> -char_('m')
                                >> -char_('M') >> char_('7')
                        ];
        }

        qi::rule<Iterator, std::string(), SpaceType> seventh_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct chord : qi::grammar<Iterator, std::string(), SpaceType>
    {
        chord() : chord::base_type(chord_)
        {
            chord_ %= lexeme[
                            char_("CDEFGAB")
                                >> -char_("#b")
                                >> -char_('m')
                                >> -(-char_('M') >> char_('7'))
                      ];
        }

        qi::rule<Iterator, std::string(), SpaceType> chord_;
    };

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
            subDiv_ %= pc_ | '(' >> div_ >> ')';

            div_ %= subDiv_ % ',';
        }

        pitch_class<Iterator, SpaceType> pc_;
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
                    chord_division_attr(),
                    SpaceType
              >
    {
        chord_division() : chord_division::base_type(div_)
        {
            subDiv_ %= chord_ | '(' >> div_ >> ')';

            div_ %= subDiv_ % ',';
        }

        chord<Iterator, SpaceType> chord_;
        qi::rule<Iterator, chord_subdivision_attr(), SpaceType> subDiv_;
        qi::rule<
                Iterator,
                chord_division_attr(),
                SpaceType
        > div_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct leadsheet_section
            : qi::grammar<
                    Iterator,
                    leadsheet_section_attr(),
                    SpaceType
              >
    {
        leadsheet_section() : leadsheet_section::base_type(section_)
        {
            name_ %= alpha >> *(alnum | char_("_'"));

            melodyLine_ %= division_ % '|';

            chordLine_ %= chordDivision_ % '|';

            section_ %= name_ >> lit(":=") >> lit('{')
                                    >> melodyLine_
                                    >> chordLine_
                              >> lit('}');
        }

        qi::rule<Iterator, std::string(), SpaceType> name_;
        division<Iterator, SpaceType> division_;
        chord_division<Iterator, SpaceType> chordDivision_;
        qi::rule<Iterator, std::vector<division_attr>(), SpaceType> melodyLine_;
        qi::rule<Iterator, std::vector<chord_division_attr>(), SpaceType> chordLine_;
        qi::rule<Iterator, leadsheet_section_attr(), SpaceType> section_;
    };
} // namespace muscr


#endif // MUSCR_SECTION_H
