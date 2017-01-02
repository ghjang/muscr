#ifndef MUSCR_PARSER_PROPERTY_H
#define MUSCR_PARSER_PROPERTY_H


#include <cstdint>
#include <set>
#include <map>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/qi.hpp>

#include "muscr/include/parser/muscr_property_attr.h"


namespace muscr::parser
{
    namespace qi = boost::spirit::qi;
    using qi::char_;
    using qi::string;
    using qi::eol;
    using qi::eps;
    using qi::lexeme;
    
    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct prop_name : qi::grammar<Iterator, std::string(), SpaceType>
    {
        prop_name() : prop_name::base_type(name_)
        {
            name_ = string("title")
                        | string("author")
                        | string("scale")
                        | string("pitchRange")
                        | string("clef")
                        | string("timeSignature")
                        | string("bpm");
        }
        
        qi::rule<Iterator, std::string(), SpaceType> name_;
    };

    template 
    <
        typename Iterator,
        typename SpaceType = qi::ascii::space_type,
        typename Attr = muscr::parser::property
    >
    struct prop_name_val_pair : qi::grammar<Iterator, Attr(), SpaceType>
    {
        prop_name_val_pair() : prop_name_val_pair::base_type(pair_)
        {
            val_ = lexeme[+(char_ - eol)];

            pair_ = '@' >> name_ >> ':' >> val_;
        }

        prop_name<Iterator, SpaceType> name_;
        qi::rule<Iterator, std::string(), SpaceType> val_;
        qi::rule<Iterator, Attr(), SpaceType> pair_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct global_properties
            : qi::grammar<
                    Iterator,
                    std::vector<muscr::parser::property>(),
                    qi::locals<std::string>,
                    SpaceType
              >
    {
        global_properties()
            : global_properties::base_type(start_)
        {
            using namespace qi::labels;
            namespace phx = boost::phoenix;
            using phx::at_c;
            using phx::insert;
            using phx::count;

            // NOTE: If a semantic action is attached to a parser,
            //       then no attributes are emitted automatically anymore.
            //       In that case, you need to insert the attribute data manually
            //       or use the auto-rule.
            start_ %= *(property_[
                                _a = at_c<0>(_1),
                                insert(phx::ref(propNames_), at_c<0>(_1))
                        ]
                        >> eps(
                                count(phx::ref(propNames_), _a) == 1
                           )
                       );
        }

        prop_name_val_pair<Iterator, SpaceType> property_;
        qi::rule<
            Iterator,
            std::vector<muscr::parser::property>(),
            qi::locals<std::string>,
            SpaceType
        > start_;

        std::multiset<std::string> propNames_;
    };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct global_properties_map
            : qi::grammar<
                    Iterator,
                    std::map<std::string, std::string>(),
                    qi::locals<std::string>,
                    SpaceType
              >
    {
        global_properties_map()
            : global_properties_map::base_type(start_)
        {
            using namespace qi::labels;
            namespace phx = boost::phoenix;
            using phx::at_c;
            using phx::insert;
            using phx::count;

            // NOTE: If a semantic action is attached to a parser,
            //       then no attributes are emitted automatically anymore.
            //       In that case, you need to insert the attribute data manually
            //       or use the auto-rule.
            start_ %= *(property_[
                                _a = at_c<0>(_1),
                                insert(phx::ref(propNames_), at_c<0>(_1))
                        ]
                        >> eps(
                                count(phx::ref(propNames_), _a) == 1
                           )
                       );
        }

        // NOTE: a list of std::pair is compatilbe with std::map.
        prop_name_val_pair<Iterator, SpaceType, std::pair<std::string, std::string>> property_;

        qi::rule<
            Iterator,
            std::map<std::string, std::string>(),
            qi::locals<std::string>,
            SpaceType
        > start_;

        std::multiset<std::string> propNames_;
    };
} // namespace muscr::parser


#endif // MUSCR_PARSER_PROPERTY_H
