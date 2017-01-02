#ifndef MUSCR_LEADSHEET_STAFF_H
#define MUSCR_LEADSHEET_STAFF_H


#include "muscr/include/parser/muscr_property.h"
#include "muscr/include/parser/section.h"

#include "muscr/include/parser/leadsheet_staff_attr.h"


namespace muscr
{
    namespace qi = boost::spirit::qi;

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct leadsheet_staff
            : qi::grammar<
                    Iterator,
                    SpaceType
              >
    {
        leadsheet_staff() : leadsheet_staff::base_type(staff_)
        {
            staff_ = properties_ >> +section_;
        }

        global_properties_map<Iterator> properties_;
        leadsheet_section<Iterator> section_;
        qi::rule<Iterator, SpaceType> staff_;
    };
} // namespace muscr


#endif // MUSCR_LEADSHEET_STAFF_H
