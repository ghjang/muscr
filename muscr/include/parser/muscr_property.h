#ifndef MUSCR_PROPERTY_H
#define MUSCR_PROPERTY_H


#include <boost/spirit/include/qi.hpp>


namespace muscr
{
    struct property
    {
        std::string name_;
        std::string value_;
    };
} // namespace muscr

BOOST_FUSION_ADAPT_STRUCT
(
    muscr::property,
    name_,
    value_
)


namespace muscr
{
    namespace detail
    {
        using namespace boost::spirit;
        using qi::ascii::string;

        template <typename Iterator>
        qi::rule<Iterator, std::string()> prop_name{
            string("title")
                | string("author")
                | string("scale")
                | string("pitchRange")
                | string("clef")
                | string("timeSignature")
                | string("bpm")
        };
    } // namespace detail

    template <typename Iterator>
    auto prop_name{ detail::prop_name<Iterator> };

    namespace detail
    {
        using namespace boost::spirit;
        using qi::ascii::char_;
        using qi::eol;
        using qi::lexeme;

        template <typename Iterator>
        qi::rule<Iterator, muscr::property(), qi::ascii::space_type> prop_str_val{
            '@' >> muscr::prop_name<Iterator> >> ':' >> lexeme[+(char_ - eol)]
        };
    } // namespace detail

    template <typename Iterator>
    auto prop_str_val{ detail::prop_str_val<Iterator> };
} // namespace muscr


#endif // MUSCR_PROPERTY_H
