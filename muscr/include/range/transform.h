#ifndef MUSCR_RANGE_TRANSFORM_H
#define MUSCR_RANGE_TRANSFORM_H


#include <cstdint>
#include <string>
#include <map>

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
        return pitch_class{ static_cast<pitch_class::number_type>(n % 12) };
    }

    template <std::int8_t MiddleCOctaveNumber = 3>
    pitch<MiddleCOctaveNumber> to_pitch(midi_note_number const& n)
    {
        std::int8_t pos = n / 12;
        if (MiddleCOctaveNumber < 5) {
            pos -= 5 - MiddleCOctaveNumber;
        } else if (MiddleCOctaveNumber > 5) {
            pos += MiddleCOctaveNumber - 5;
        }
        return pitch<MiddleCOctaveNumber>{ to_pitch_class(n), pos };
    }

    template <std::int8_t MiddleCOctaveNumber>
    midi_note_number to_midi_note_number(pitch<MiddleCOctaveNumber> const& p)
    {
        std::int8_t pos{};
        if (MiddleCOctaveNumber < 5) {
            pos = p.octavePos_ + (5 - MiddleCOctaveNumber);
        } else if (MiddleCOctaveNumber > 5) {
            pos = p.octavePos_ - (MiddleCOctaveNumber - 5);
        } else {
            pos = p.octavePos_;
        }
        return { static_cast<midi_note_number::number_type>(p.pc_ + pos * 12) };
    }

    template <std::int8_t MiddleCOctaveNumber>
    std::string to_string(pitch<MiddleCOctaveNumber> const& p, bool useFlat = true)
    {
        std::string s{ to_string(p.pc_, useFlat) };
        s.append({ static_cast<char>('0' + p.octavePos_) });
        return s;
    }
} // namespace muscr::range


#endif // MUSCR_RANGE_TRANSFORM_H
