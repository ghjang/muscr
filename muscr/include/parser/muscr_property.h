#ifndef MUSCR_PROPERTY_H
#define MUSCR_PROPERTY_H


#include <boost/spirit/include/qi.hpp>


struct property
{
    std::string name_;
    std::string value_;
};

BOOST_FUSION_ADAPT_STRUCT
(
    property,
    name_,
    value_
)


#endif // MUSCR_PROPERTY_H
