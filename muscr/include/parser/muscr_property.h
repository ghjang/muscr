#ifndef MUSCR_PROPERTY_H
#define MUSCR_PROPERTY_H


#include <cstdint>
#include <set>

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>

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
    namespace detail
    {
        namespace qi = boost::spirit::qi;
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
        namespace qi = boost::spirit::qi;
        using qi::ascii::char_;
        using qi::eol;
        using qi::lexeme;

        template <typename Iterator, typename SpaceType>
        qi::rule<Iterator, muscr::property(), SpaceType> prop_str_val{
            '@' >> muscr::prop_name<Iterator> >> ':' >> lexeme[+(char_ - eol)]
        };
    } // namespace detail

    template <typename Iterator, typename SpaceType = boost::spirit::qi::ascii::space_type>
    auto prop_str_val{ detail::prop_str_val<Iterator, SpaceType> };

    template <typename Iterator, typename SpaceType = boost::spirit::qi::ascii::space_type>
    struct global_properties
            : boost::spirit::qi::grammar<
                    Iterator,
                    std::vector<muscr::property>(),
                    boost::spirit::qi::locals<std::string>,
                    SpaceType
              >
    {
        global_properties()
            : global_properties::base_type(start_)
        {
            namespace qi = boost::spirit::qi;
            using qi::eps;
            using namespace qi::labels;
            namespace phx = boost::phoenix;
            using phx::at_c;
            using phx::insert;
            using phx::push_back;
            using phx::count;

            property_ = prop_str_val<Iterator, SpaceType>;

            // NOTE: if a semantic action is attached to a parser,
            //       then no properties are emitted automatically anymore.
            start_ = *(property_[
                            _a = at_c<0>(_1),
                            insert(phx::ref(propNames_), at_c<0>(_1)),
                            push_back(_val, _1) // need to push it manually.
                       ]
                       >> eps(
                            count(phx::ref(propNames_), _a) == 1
                          )
                      );
        }

        boost::spirit::qi::rule<Iterator, muscr::property(), SpaceType> property_;
        boost::spirit::qi::rule<
            Iterator,
            std::vector<muscr::property>(),
            boost::spirit::qi::locals<std::string>,
            SpaceType
        > start_;

        std::multiset<std::string> propNames_;
    };
} // namespace muscr


#endif // MUSCR_PROPERTY_H
