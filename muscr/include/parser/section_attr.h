#ifndef MUSCR_PARSER_SECTION_ATTR_H
#define MUSCR_PARSER_SECTION_ATTR_H


#include <string>
#include <vector>

#include <boost/variant/recursive_variant.hpp>

#include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#include <boost/fusion/include/define_struct_inline.hpp>


namespace muscr
{
    using subdivision_attr = boost::make_recursive_variant<
                                    std::string,
                                    std::vector<boost::recursive_variant_>
                             >::type;

    using division_attr = std::vector<subdivision_attr>;


    using chord_subdivision_attr = boost::make_recursive_variant<
                                        std::string,
                                        std::vector<boost::recursive_variant_>
                                   >::type;

    using chord_division_attr = std::vector<chord_subdivision_attr>;


    BOOST_FUSION_DEFINE_STRUCT_INLINE(
        leadsheet_section_attr,
        (std::string, name_)
        (std::vector<division_attr>, melodyLine_)
        (std::vector<chord_division_attr>, chordLine_)
    )
} // namespace muscr


#endif // MUSCR_PARSER_SECTION_ATTR_H
