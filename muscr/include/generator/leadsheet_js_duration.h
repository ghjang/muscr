#ifndef MUSCR_GENERATOR_LEADSHEET_JS_DURATION
#define MUSCR_GENERATOR_LEADSHEET_JS_DURATION


#include <string>
#include <map>

#include "muscr/include/division_ratio.h"


namespace muscr::generator::ljs
{
    using namespace muscr::division_ratio;

    std::map<ratio<>, std::string> const note_ratio_to_str_map{
        { whole_note,       "w" },
        { half_note,        "h" },
        { quater_note,      "q" },
        { eighth_note,      "8" },
        { sixteenth_note,   "16" },
    };

    std::map<ratio<10>, std::string> const rest_ratio_to_str_map{
        { whole_rest,       "wr" },
        { half_rest,        "hr" },
        { quater_rest,      "qr" },
        { eighth_rest,      "8r" },
        { sixteenth_rest,   "16r" }
    };

    auto ratio_to_str(ratio<> const& r) { return note_ratio_to_str_map.at(r); }
    auto ratio_to_str(ratio<10> const& r) { return rest_ratio_to_str_map.at(r); }
} // namespace muscr::generator::ljs


#endif // MUSCR_GENERATOR_LEADSHEET_JS_DURATION
