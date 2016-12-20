#ifndef MUSCR_SECTION_H
#define MUSCR_SECTION_H


#include <string>

#include <boost/spirit/include/qi.hpp>


namespace muscr
{
    namespace qi = boost::spirit::qi;
    using qi::string;
    using qi::char_;

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
} // namespace muscr


#endif // MUSCR_SECTION_H
