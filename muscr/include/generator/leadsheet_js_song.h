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
            std::string pitchClass_;
            std::string modifier_;              // major modifiers like 'm', 'aug' and 'dim'.
            std::string accessaryModifier_;     // accessary modifiers like '7', 'M7', '9' etc.
            std::uint8_t beatPos_;
        };

        struct note_attr
        {
            std::string pitchClass_;
            std::int8_t octavePos_;
            std::string duration_;
        };
    }
}

// NOTE: BOOST_FUSION_DEFINE_STRUCT was NOT WORKING in the nested namespace like the muscr::ljs.
//          Need to re-test it?

BOOST_FUSION_ADAPT_STRUCT(
    muscr::ljs::chord_attr,
    (std::string, pitchClass_)
    (std::string, modifier_)
    (std::string, accessaryModifier_)
    (std::uint8_t, beatPos_)
)

BOOST_FUSION_ADAPT_STRUCT(
    muscr::ljs::note_attr,
    (std::string, pitchClass_)
    (std::int8_t, octavePos_)
    (std::string, duration_)
)


namespace muscr
{
    namespace karma = boost::spirit::karma;
    using karma::lit;
    using karma::char_;
    using karma::string;
    using karma::ushort_;
    using karma::short_;

    namespace ljs
    {
        template <typename OutIter>
        struct chord : karma::grammar<OutIter, chord_attr()>
        {
            chord() : chord::base_type(chordObj_)
            {
                pc_ = char_("CDEFGAB") << -char_("#b");

                modifier_ = &char_('m') << 'm';

                accessaryModifier_ = &string("7") << '7'
                                        | &string("M7") << "M7";

                chordObj_ = lit("{ ")
                                << "p : '" << pc_ << "', "
                                << "ch : '" << -modifier_ << -accessaryModifier_ << "', "
                                << "beat : " << ushort_
                          << lit(" }");
            }

            karma::rule<OutIter, std::string()> pc_;
            karma::rule<OutIter, std::string()> modifier_;
            karma::rule<OutIter, std::string()> accessaryModifier_;
            karma::rule<OutIter, chord_attr()> chordObj_;
        };

        template <typename OutIter>
        struct note : karma::grammar<OutIter, note_attr()>
        {
            note() : note::base_type(noteObj_)
            {
                pc_ = char_("CDEFGAB") << -char_("#b");

                // TODO: add constraint rules HERE for the octave position and duration.

                noteObj_ = lit("{ ")
                                << "keys: ['" << pc_ << '/' << short_ << "'], "
                                << "duration: '" << string << '\''
                         << lit(" }");
            }

            karma::rule<OutIter, std::string()> pc_;
            karma::rule<OutIter, note_attr()> noteObj_;
        };
    } // namespace ljs

} // namespace muscr


#endif // MUSCR_GENERATOR_LEADSHEET_JS_SONG_H
