#ifndef MUSCR_GENERATOR_LEADSHEET_JS_SONG_H
#define MUSCR_GENERATOR_LEADSHEET_JS_SONG_H


#include <string>

#include <boost/spirit/include/karma.hpp>

#include "muscr/include/generator/leadsheet_js_song_attr.h"


namespace muscr::generator::ljs
{
    namespace karma = boost::spirit::karma;
    using karma::lit;
    using karma::char_;
    using karma::string;
    using karma::ushort_;
    using karma::short_;

    template <typename OutIter>
    struct chord : karma::grammar<OutIter, chord_attr()>
    {
        chord() : chord::base_type(chordObj_)
        {
            pc_ = char_("CDEFGAB") << -char_("#b");

            modifier_ = &char_('m') << 'm';

            // NOTE: '& predicate and karma::string' combination seems not to consume attributes.
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
                            << "keys : ['" << pc_ << '/' << short_ << "'], "
                            << "duration : '" << string << '\''
                        << lit(" }");
        }

        karma::rule<OutIter, std::string()> pc_;
        karma::rule<OutIter, note_attr()> noteObj_;
    };

    template <typename OutIter>
    struct bar : karma::grammar<OutIter, bar_attr()>
    {
        bar() : bar::base_type(barObj_)
        {
            barObj_ = lit("{ ")
                            << "chords : [" << (chord_ % ", ") << "], "
                            << "melody : [" << (note_ % ", ") << ']'
                    << lit(" }");
        }

        chord<OutIter> chord_;
        note<OutIter> note_;
        karma::rule<OutIter, bar_attr()> barObj_;
    };

    template <typename OutIter>
    struct section : karma::grammar<OutIter, section_attr()>
    {
        section() : section::base_type(sectionObj_)
        {
            sectionObj_ = lit("{ ")
                                << "name : '" << string << "', "
                                << "bars : [" << (bar_ % ", ") << ']'
                        << lit(" }");
        }

        bar<OutIter> bar_;
        karma::rule<OutIter, section_attr()> sectionObj_;
    };

    template <typename OutIter>
    struct leadsheet_staff : karma::grammar<OutIter, leadsheet_staff_attr()>
    {
        leadsheet_staff() : leadsheet_staff::base_type(lsObj_)
        {
            lsObj_ = lit("{ ")
                            << "title : '" << string << "', "
                            << "composer : '" << string << "', "
                            << "keySignature : '" << string << "', "
                            << "time : '" << string << "', "
                            << "changes : [" << (section_ % ", ") << ']'
                    << lit(" }");
        }

        section<OutIter> section_;
        karma::rule<OutIter, leadsheet_staff_attr()> lsObj_;
    };
} // namespace muscr::generator::ljs


#endif // MUSCR_GENERATOR_LEADSHEET_JS_SONG_H
