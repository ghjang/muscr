#ifndef MUSCR_TO_LJS_CONVERSION_H
#define MUSCR_TO_LJS_CONVERSION_H


#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <stack>

#include <boost/variant/recursive_variant.hpp>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

#include "muscr/include/parser/leadsheet_staff_attr.h"
#include "muscr/include/generator/leadsheet_js_song.h"
#include "muscr/include/generator/leadsheet_js_duration.h"


namespace muscr::detail
{
    using muscr::division_ratio::ratio;

    auto get_unit_note_ratio(std::uint16_t denom)
    {
        switch (denom) {
            case 4 :
                return ratio<>{ 1, 1 };
            case 8 :
                return ratio<>{ 1, 2 };
            case 2 :
                return ratio<>{ 2, 1 };
            case 1 :
                return ratio<>{ 4, 1 };
            case 16 :
                return ratio<>{ 1, 4 };
            default :
                // TODO: throw an exception.
                return ratio<>{};
        }
    }

    auto get_bar_duration_ratio(std::string & timeSigStr)
    {
        namespace qi = boost::spirit::qi;
        using qi::ushort_;
        using time_sig_attr = std::pair<std::uint16_t, std::uint16_t>;

        qi::rule<std::string::iterator, time_sig_attr(), qi::ascii::space_type> const timeSig_
            = ushort_ >> '/' >> (ushort_(4) | ushort_(8) | ushort_(2) | ushort_(1) | ushort_(16));

        auto begin = timeSigStr.begin();
        auto end = timeSigStr.end();
        time_sig_attr attr;
        bool r = qi::phrase_parse(begin, end, timeSig_, qi::ascii::space, attr);
        if (!r || begin != end) {
            // TODO: throw an exception.
        }

        return (ratio<>{ attr.first } * get_unit_note_ratio(attr.second)).to_lowest_term();
    }

    auto parse_to_chord_attr(std::string chordStr)
    {
        namespace qi = boost::spirit::qi;
        using qi::char_;
        using qi::as_string;
        using muscr::generator::ljs::chord_attr;

        qi::rule<std::string::iterator, chord_attr()> const chord_
            = as_string[char_("CDEFGAB") >> -char_("#b")]
                >> as_string[-char_('m')]
                >> as_string[-(-char_('M') >> char_('7'))];

        auto begin = chordStr.begin();
        auto end = chordStr.end();
        chord_attr attr;
        bool r = qi::parse(begin, end, chord_, attr);
        if (!r || begin != end) {
            // TODO: throw an exception.
        }

        return attr;
    }

    struct melody_division_visitor : boost::static_visitor<>
    {
        melody_division_visitor(muscr::generator::ljs::bar_attr & destBar,
                                int defaultPitchRange,
                                int topLevelSubDivCnt,
                                ratio<> const& barDurationRatio)
            : destBar_{ destBar }
            , defaultPitchRange_{ defaultPitchRange }
        {
            auto r = barDurationRatio / ratio<>{ topLevelSubDivCnt };
            subDivRatioStack_.push(r.to_lowest_term());
        }

        void operator () (std::string const& pitchClass)
        {
            muscr::generator::ljs::note_attr attr{
                pitchClass,
                static_cast<std::int8_t>(defaultPitchRange_),
                muscr::generator::ljs::ratio_to_str(subDivRatioStack_.top())
            };

            destBar_.notes_.push_back(std::move(attr));
        }

        template <typename T>
        void operator () (std::vector<T> const& subDiv)
        {
            auto top = subDivRatioStack_.top();
            auto nextSubDivRatio = top / ratio<>{ static_cast<int>(subDiv.size()) };
            subDivRatioStack_.push(nextSubDivRatio.to_lowest_term());

            for (auto & e : subDiv) {
                boost::apply_visitor(*this, e);
            }

            subDivRatioStack_.pop();
        }

        muscr::generator::ljs::bar_attr & destBar_;
        int const defaultPitchRange_;
        std::stack<ratio<>> subDivRatioStack_;
    };

    struct chord_division_visitor : boost::static_visitor<>
    {
        chord_division_visitor(muscr::generator::ljs::bar_attr & destBar,
                               int topLevelSubDivCnt,
                               ratio<> const& barDurationRatio)
            : destBar_{ destBar }
        {
            auto r = barDurationRatio / ratio<>{ topLevelSubDivCnt };
            subDivRatioStack_.push(r.to_lowest_term());
        }

        void operator () (std::string const& chord)
        {
            auto attr = parse_to_chord_attr(chord);
            attr.beatPos_ = 1; // FIXME
            destBar_.chords_.push_back(std::move(attr));
        }

        template <typename T>
        void operator () (std::vector<T> const& subDiv)
        {
            auto top = subDivRatioStack_.top();
            auto nextSubDivRatio = top / ratio<>{ static_cast<int>(subDiv.size()) };
            subDivRatioStack_.push(nextSubDivRatio.to_lowest_term());

            for (auto e : subDiv) {
                boost::apply_visitor(*this, e);
            }

            subDivRatioStack_.pop();
        }

        muscr::generator::ljs::bar_attr & destBar_;
        std::stack<ratio<>> subDivRatioStack_;
    };

    void convert_melody_division(muscr::generator::ljs::leadsheet_staff_attr & destAttr,
                                 muscr::generator::ljs::bar_attr & destBar,
                                 int defaultPitchRange,
                                 muscr::parser::division_attr const& srcBar)
    {
        int topLevelSubDivCnt = srcBar.size();
        auto barDurationRatio = get_bar_duration_ratio(destAttr.time_);
        for (auto & div : srcBar) {
            melody_division_visitor v{ destBar, defaultPitchRange, topLevelSubDivCnt, barDurationRatio };
            boost::apply_visitor(v, div);
        }
    }

    void convert_chord_division(muscr::generator::ljs::leadsheet_staff_attr & destAttr,
                                muscr::generator::ljs::bar_attr & destBar,
                                muscr::parser::chord_division_attr const& srcBar)
    {
        int topLevelSubDivCnt = srcBar.size();
        auto barDurationRatio = get_bar_duration_ratio(destAttr.time_);
        for (auto & div : srcBar) {
            chord_division_visitor v{ destBar, topLevelSubDivCnt, barDurationRatio };
            boost::apply_visitor(v, div);
        }
    }
} // namespace muscr::detail

namespace muscr
{
    auto to_leadsheet_attr(std::string & muscrStr)
    {
        namespace qi = boost::spirit::qi;
        muscr::parser::leadsheet_staff<std::string::iterator> staff_;
        muscr::parser::leadsheet_staff_attr srcAttr;

        auto begin = muscrStr.begin();
        auto end = muscrStr.end();
        bool parseResult = qi::phrase_parse(begin, end, staff_, qi::ascii::space, srcAttr);
        if (!parseResult || begin != end) {
            // TODO: throw an exception
        }
        return srcAttr;
    }

    auto to_ljs_song_data_attr(muscr::parser::leadsheet_staff_attr srcAttr)
    {
        muscr::generator::ljs::leadsheet_staff_attr destAttr;

        destAttr.title_         = srcAttr.properties_["title"];
        destAttr.composer_      = srcAttr.properties_["author"];
        destAttr.keySignature_  = srcAttr.properties_["scale"];
        destAttr.time_          = srcAttr.properties_["timeSignature"];

        auto const defaultPitchRange = std::stoi(srcAttr.properties_["pitchRange"]);

        for (auto & section : srcAttr.sections_) {
            assert(section.melodyLine_.size() == section.chordLine_.size());
            if (section.melodyLine_.size() != section.chordLine_.size()) {
                throw std::runtime_error("[muscr] different bar size");
            }

            int const barCnt = section.melodyLine_.size();
            muscr::generator::ljs::section_attr sa;
            sa.name_ = section.name_;
            sa.bars_.resize(barCnt);

            for (int i = 0; i < barCnt; ++i) {
                detail::convert_melody_division(
                    destAttr, sa.bars_[i], defaultPitchRange, section.melodyLine_[i]
                );
            }

            for (int i = 0; i < barCnt; ++i) {
                detail::convert_chord_division(
                    destAttr, sa.bars_[i], section.chordLine_[i]
                );
            }

            destAttr.sections_.push_back(std::move(sa));
        }

        return destAttr;
    }

    std::string to_ljs_song_data_str(muscr::generator::ljs::leadsheet_staff_attr attr)
    {
        using sink_type = std::back_insert_iterator<std::string>;
        muscr::generator::ljs::leadsheet_staff<sink_type> ls_;
        std::string ljsStr;
        std::back_insert_iterator<std::string> out(ljsStr);
        bool genResult = boost::spirit::karma::generate(out, ls_, attr);
        if (!genResult) {
            // TODO: throw an exception.
        }
        return ljsStr;
    }

    std::string generate_ljs_song_data_module_str(std::string & muscrStr)
    {
        auto srcAttr = to_leadsheet_attr(muscrStr);
        auto destAttr = to_ljs_song_data_attr(srcAttr);
        std::ostringstream oss;
        oss << "define(function() { return " << to_ljs_song_data_str(destAttr) << "; })";
        return oss.str();
    }
} // namespace muscr


#endif // MUSCR_TO_LJS_CONVERSION_H
