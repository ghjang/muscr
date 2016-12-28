#ifndef MUSCR_GENERATOR_LEADSHEET_JS_SONG_H
#define MUSCR_GENERATOR_LEADSHEET_JS_SONG_H


#include <cstdint>
#include <string>

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>

#include <boost/spirit/include/karma.hpp>


namespace muscr
{
    namespace ljs
    {
        struct chord_attr
        {
            std::string value_;
            std::uint8_t beatPos_;
        };
    }
}

BOOST_FUSION_ADAPT_STRUCT(
    muscr::ljs::chord_attr,
    (std::string, value_)
    (std::uint8_t, beatPos_)
)


namespace muscr
{
    namespace karma = boost::spirit::karma;
    using karma::lit;
    using karma::char_;
    using karma::ushort_;

    namespace ljs
    {
        template <typename OutIter>
        struct chord : karma::grammar<OutIter, chord_attr()>
        {
            chord() : chord::base_type(chordObj_)
            {
                chord_ = "p : '" << char_("CDEFGAB") << -char_("#b") << "', "
                       << "ch : '" << -char_('m') << -(-char_('M') << char_('7')) << '\'';

                chordObj_ = lit("{ ")
                                << chord_
                                << ", beat : " << ushort_
                          << lit(" }");
            }

            karma::rule<OutIter, std::string()> chord_;
            karma::rule<OutIter, chord_attr()> chordObj_;
        };
    } // namespace ljs

} // namespace muscr


#endif // MUSCR_GENERATOR_LEADSHEET_JS_SONG_H
