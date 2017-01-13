#ifndef MUSCR_RANGE_TRANSFORM_H
#define MUSCR_RANGE_TRANSFORM_H


#include "muscr/include/range/pitch.h"
#include "muscr/include/range/midi_note.h"


namespace muscr::range
{
    inline std::string to_string(pitch_class const& pc, bool useFlat = true)
    {
        using pc_to_str_map_t = std::map<pitch_class::number_type, std::string>;
        static pc_to_str_map_t const map{
            { 0,    "C"     },
            { 1,    "Db"    },
            { 2,    "D"     },
            { 3,    "Eb"    },
            { 4,    "E"     },
            { 5,    "F"     },
            { 6,    "Gb"    },
            { 7,    "G"     },
            { 8,    "Ab"    },
            { 9,    "A"     },
            { 10,   "Bb"    },
            { 11,   "B"     }
        };

        auto iter = map.find(pc);
        if (map.end() == iter) {
            // TODO: throw an exception
        }
        if (useFlat || iter->second.length() == 1) {
            return iter->second;
        }
        assert(iter->second.length() == 2);
        iter = map.find(pc - 1);
        if (map.end() == iter) {
            // TODO: throw an exception
        }
        assert(iter->second.length() == 1);
        return std::string{ iter->second.front(), '#' };
    }

    inline pitch_class to_pitch_class(std::string const& s)
    {
        using str_to_pc_map_t = std::map<std::string, pitch_class::number_type>;
        static str_to_pc_map_t const map{
            { "C",      0 },
            { "C#",     1 },    { "DB",    1 },
            { "D",      2 },
            { "D#",     3 },    { "EB",    3 },
            { "E",      4 },
            { "F",      5 },
            { "F#",     6 },    { "GB",    6 },
            { "G",      7 },
            { "G#",     8 },    { "AB",    8 },
            { "A",      9 },
            { "A#",     10 },   { "BB",     10},
            { "B",      11 }
        };

        std::string key = boost::to_upper_copy(s);
        auto iter = map.find(key);
        if (map.end() == iter) {
            // TODO: throw an exception
        }
        return pitch_class{ iter->second };
    }

    inline pitch_class to_pitch_class(midi_note_number const& n)
    {
        return pitch_class{};
    }
} // namespace muscr::range


#endif // MUSCR_RANGE_TRANSFORM_H
