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


#endif // MUSCR_PROPERTY_H
