#ifndef MUSCR_GENERATOR_LEADSHEET_JS_SONG_ATTR_H
#define MUSCR_GENERATOR_LEADSHEET_JS_SONG_ATTR_H


#include <cstdint>
#include <string>
#include <vector>

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>


namespace muscr::generator::ljs
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

    struct bar_attr
    {
        std::vector<chord_attr> chords_;
        std::vector<note_attr> notes_;
    };

    struct section_attr
    {
        std::string name_;
        std::vector<bar_attr> bars_;
    };

    struct leadsheet_staff_attr
    {
        std::string title_;
        std::string composer_;
        std::string keySignature_;
        std::string time_;
        std::vector<section_attr> sections_;
    };
} // namespace muscr::generator::ljs


// NOTE: BOOST_FUSION_DEFINE_STRUCT was NOT WORKING in the nested namespace like the muscr::ljs.
//          Need to re-test it?

BOOST_FUSION_ADAPT_STRUCT(
    muscr::generator::ljs::chord_attr,
    (std::string, pitchClass_)
    (std::string, modifier_)
    (std::string, accessaryModifier_)
    (std::uint8_t, beatPos_)
)

BOOST_FUSION_ADAPT_STRUCT(
    muscr::generator::ljs::note_attr,
    (std::string, pitchClass_)
    (std::int8_t, octavePos_)
    (std::string, duration_)
)

BOOST_FUSION_ADAPT_STRUCT(
    muscr::generator::ljs::bar_attr,
    (std::vector<muscr::generator::ljs::chord_attr>, chords_)
    (std::vector<muscr::generator::ljs::note_attr>, notes_)
)

BOOST_FUSION_ADAPT_STRUCT(
    muscr::generator::ljs::section_attr,
    (std::string, name_)
    (std::vector<muscr::generator::ljs::bar_attr>, bars_)
)

BOOST_FUSION_ADAPT_STRUCT(
    muscr::generator::ljs::leadsheet_staff_attr,
    (std::string, title_)
    (std::string, composer_)
    (std::string, keySignature_)
    (std::string, time_)
    (std::vector<muscr::generator::ljs::section_attr>, sections_)
)


#endif // MUSCR_GENERATOR_LEADSHEET_JS_SONG_ATTR_H
