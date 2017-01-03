#ifndef MUSCR_TO_LJS_CONVERSION_H
#define MUSCR_TO_LJS_CONVERSION_H


#include <string>
#include <vector>

#include <boost/variant/recursive_variant.hpp>

#include "muscr/include/parser/leadsheet_staff_attr.h"
#include "muscr/include/generator/leadsheet_js_song_attr.h"


namespace muscr
{
    namespace detail
    {
        struct melody_division_visitor : boost::static_visitor<>
        {
            melody_division_visitor(std::vector<muscr::generator::ljs::bar_attr> & bars)
                : bars_{ bars }
            { }

            void operator () (std::string const& pitchClass) const
            {

            }

            void operator () (muscr::parser::subdivision_attr const& attr) const
            {

            }

            std::vector<muscr::generator::ljs::bar_attr> & bars_;
        };

        struct chord_division_visitor : boost::static_visitor<>
        {
            chord_division_visitor(std::vector<muscr::generator::ljs::bar_attr> & bars)
                : bars_{ bars }
            { }

            void operator () (std::string const& chord) const
            {

            }

            void operator () (muscr::parser::chord_subdivision_attr const& attr) const
            {

            }

            std::vector<muscr::generator::ljs::bar_attr> & bars_;
        };

        void convert_melody_division(muscr::generator::ljs::leadsheet_attr const& destAttr,
                                     std::vector<muscr::generator::ljs::bar_attr> & bars,
                                     muscr::parser::division_attr const& bar)
        {
            auto topLevelSubDivCnt = bar.size();
            for (auto & div : bar) {
                boost::apply_visitor(
                    melody_division_visitor{ bars },
                    div
                );
            }
        }

        void convert_chord_division(muscr::generator::ljs::leadsheet_attr const& destAttr,
                                    std::vector<muscr::generator::ljs::bar_attr> & bars,
                                    muscr::parser::chord_division_attr const& bar)
        {
            auto topLevelSubDivCnt = bar.size();
            for (auto & div : bar) {
                boost::apply_visitor(
                    chord_division_visitor{ bars },
                    div
                );
            }
        }
    } // namespace muscr::detail

    auto to_ljs_song_data(muscr::parser::leadsheet_staff_attr srcAttr)
    {
        muscr::generator::ljs::leadsheet_attr destAttr;

        destAttr.title_         = srcAttr.properties_["title"];
        destAttr.composer_      = srcAttr.properties_["author"];
        destAttr.keySignature_  = srcAttr.properties_["scale"];
        destAttr.time_          = srcAttr.properties_["timeSignature"];

        for (auto & section : srcAttr.sections_) {
            muscr::generator::ljs::section_attr sa;

            sa.name_ = section.name_;

            for (auto & bar : section.melodyLine_) {
                detail::convert_melody_division(destAttr, sa.bars_, bar);
            }

            for (auto & bar : section.chordLine_) {
                detail::convert_chord_division(destAttr, sa.bars_, bar);
            }

            destAttr.sections_.push_back(std::move(sa));
        }

        return destAttr;
    }
} // namespace muscr


#endif // MUSCR_TO_LJS_CONVERSION_H
