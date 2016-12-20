#ifndef MUSCR_SECTION_H
#define MUSCR_SECTION_H


#include <string>

#include <boost/spirit/include/qi.hpp>


namespace muscr
{
    namespace detail
    {
        namespace qi = boost::spirit::qi;
        using qi::char_;

        template <typename Iterator>
        qi::rule<Iterator, std::string()> pitch_class{
            char_("CDEFGAB") >> -char_("#b")    
        };
    } // namespace detail

    template <typename Iterator>
    auto pitch_class{ detail::pitch_class<Iterator> };
} // namespace muscr


#endif // MUSCR_SECTION_H
