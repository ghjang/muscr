#ifndef MUSCR_PITCH_CLASS_H
#define MUSCR_PITCH_CLASS_H


#include <cassert>
#include <cstdint>
#include <string>
#include <map>

#include <boost/algorithm/string.hpp>

#include <range/v3/all.hpp>


namespace muscr::range
{
    struct pitch_class
    {
    public:
        using number_type = std::uint8_t;

        pitch_class(number_type pc)
            : pc_{ pc }
        {
            assert(pc >= 0 && pc <= 11);
            // TODO : throw an exception
        }

        operator number_type () const
        { return pc_; }

        pitch_class & operator ++ ()
        {
            if (11 == pc_) {
                pc_ = 0;
            } else {
                ++pc_;
            }
            return *this;
        }

        pitch_class operator ++ (int)
        {
            pitch_class tmp(*this);
            operator ++ ();
            return tmp;
        }

    private:
        number_type pc_;
    };

    pitch_class to_pitch_class(std::string const& s)
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

    std::string to_string(pitch_class const& pc, bool useFlat = true)
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

    struct pitch_class_view
            : ranges::view_facade<pitch_class_view>
    {
    public:
        pitch_class_view(pitch_class::number_type start = 0)
            : pc_{ start }
        { }

        pitch_class_view(std::string const& start)
            : pc_{ to_pitch_class(start) }
        { }

    private:
        pitch_class pc_;

    private: 
        friend struct ranges::range_access;

        // NOTE: It seems that a 'cursor' should be default constructable.
        //          The cursor's member must have certain conditions.
        struct cursor
        {
            cursor(pitch_class * pc = 0)
                : pPc_{ pc }
            { }

            pitch_class get() const
            { return *pPc_; }

            bool equal(cursor const& /* rhs */) const
            { return false; } // infinite range

            void next()
            { ++*pPc_; }

            pitch_class * pPc_;
        };

        cursor begin_cursor()
        { return { &pc_ }; }

        cursor end_cursor()
        { return { &pc_ }; }
    };
} // namespace muscr::range


#endif // MUSCR_PITCH_CLASS_H
