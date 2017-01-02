#ifndef MUSCR_PARSER_PROPERTY_ATTR_H
#define MUSCR_PARSER_PROPERTY_ATTR_H


#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>


BOOST_FUSION_DEFINE_STRUCT
(
    (muscr), property,
    (std::string, name_)
    (std::string, value_)
)


#endif // MUSCR_PARSER_PROPERTY_ATTR_H
