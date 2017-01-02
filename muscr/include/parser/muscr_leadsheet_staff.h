#ifndef MUSCR_LEADSHEET_SCORE_H
#define MUSCR_LEADSHEET_SCORE_H


#include "muscr/include/parser/muscr_property.h"
#include "muscr/include/parser/muscr_section.h"


namespace muscr
{
    namespace qi = boost::spirit::qi;

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct leadsheet_score
            : qi::grammar<
                    Iterator,
                    SpaceType
              >
    {
        leadsheet_score() : leadsheet_score::base_type(score_)
        {
            score_ = properties_ >> +section_;
        }

        global_properties_map<Iterator> properties_;
        leadsheet_section<Iterator> section_;
        qi::rule<Iterator, SpaceType> score_;
    };
} // namespace muscr


#endif // MUSCR_LEADSHEET_SCORE_H
