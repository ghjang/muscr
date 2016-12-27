#ifndef MUSCR_PROPERTY_H
#define MUSCR_PROPERTY_H


#include <cstdint>
#include <set>
#include <map>

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/qi.hpp>


BOOST_FUSION_DEFINE_STRUCT
(
    (muscr), property,
    (std::string, name_)
    (std::string, value_)
)


namespace muscr
{
    namespace qi = boost::spirit::qi;
    
    namespace detail
    {
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
        using qi::ascii::char_;
        using qi::eol;
        using qi::lexeme;

        template <typename Iterator, typename SpaceType, typename Attr>
        qi::rule<Iterator, Attr(), SpaceType> prop_str_val{
            '@' >> muscr::prop_name<Iterator> >> ':' >> lexeme[+(char_ - eol)]
        };
    } // namespace detail

    template
    <
        typename Iterator,
        typename SpaceType = qi::ascii::space_type,
        typename Attr = muscr::property
    >
    auto prop_str_val{ detail::prop_str_val<Iterator, SpaceType, Attr> };

    template <typename Iterator, typename SpaceType = qi::ascii::space_type>
    struct global_properties
            : qi::grammar<
                    Iterator,
                    std::vector<muscr::property>(),
                    qi::locals<std::string>,
                    SpaceType
              >
    {
        global_properties()
            : global_properties::base_type(start_)
        {
            using qi::eps;
            using namespace qi::labels;
            namespace phx = boost::phoenix;
            using phx::at_c;
            using phx::insert;
            using phx::count;

            property_ = prop_str_val<Iterator, SpaceType>;

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

        qi::rule<Iterator, muscr::property(), SpaceType> property_;
        qi::rule<
            Iterator,
            std::vector<muscr::property>(),
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
            using qi::eps;
            using namespace qi::labels;
            namespace phx = boost::phoenix;
            using phx::at_c;
            using phx::insert;
            using phx::count;

            // NOTE: a list of std::pair is compatilbe with std::map.
            property_ = prop_str_val<Iterator, SpaceType, std::pair<std::string, std::string>>;

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

        qi::rule<
            Iterator,
            std::pair<std::string, std::string>(),
            SpaceType
        > property_;

        qi::rule<
            Iterator,
            std::map<std::string, std::string>(),
            qi::locals<std::string>,
            SpaceType
        > start_;

        std::multiset<std::string> propNames_;
    };
} // namespace muscr


#endif // MUSCR_PROPERTY_H
