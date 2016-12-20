#ifndef MUSCR_SECTION_H
#define MUSCR_SECTION_H


#include <cstdint>
#include <string>

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

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct division
            : qi::grammar<
                    Iterator,
                    SpaceType
              >
    {
        division() : division::base_type(div_)
        {
            pc_ = pitch_class<Iterator>;

            subDiv_ = '(' >> div_ >> ')';

            div_ = (pc_ | subDiv_) % ',';
        }

        qi::rule<Iterator, std::string()> pc_;
        qi::rule<Iterator, SpaceType> subDiv_;
        qi::rule<
                Iterator,
                SpaceType
        > div_;
    };        

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct chord_division
            : qi::grammar<
                    Iterator,
                    SpaceType
              >
    {
        chord_division() : chord_division::base_type(div_)
        {
            chord_ = chord<Iterator>;

            subDiv_ = '(' >> div_ >> ')';

            div_ = (chord_ | subDiv_) % ',';
        }

        qi::rule<Iterator, std::string()> chord_;
        qi::rule<Iterator, SpaceType> subDiv_;
        qi::rule<
                Iterator,
                SpaceType
        > div_;
    };        
} // namespace muscr


#endif // MUSCR_SECTION_H
