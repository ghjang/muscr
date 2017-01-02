#ifndef MUSCR_PARSER_LEADSHEET_STAFF_ATTR_H
#define MUSCR_PARSER_LEADSHEET_STAFF_ATTR_H


#include <string>
#include <vector>
#include <map>

#include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#include <boost/fusion/include/define_struct_inline.hpp>

#include "muscr/include/parser/muscr_property_attr.h"
#include "muscr/include/parser/section_attr.h"


namespace muscr::parser
{
    using prop_name_val_map_t = std::map<std::string, std::string>;
    using sections_t = std::vector<muscr::parser::leadsheet_section_attr>;

    BOOST_FUSION_DEFINE_STRUCT_INLINE(
        leadsheet_staff_attr,
        (prop_name_val_map_t, properties_)
        (sections_t, sections_)
    )
} // namespace muscr::parser


#endif // MUSCR_PARSER_LEADSHEET_STAFF_ATTR_H
