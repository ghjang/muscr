#ifndef MUSCR_PITCH_CLASS_H
#define MUSCR_PITCH_CLASS_H


#include <cassert>
#include <cstdint>
#include <string>

#include <boost/algorithm/string.hpp>

#include <range/v3/all.hpp>


namespace muscr::range
{
    struct pitch_class
    {
    public:
        using number_type = std::uint8_t;

        pitch_class(number_type pc = 0)
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

#ifndef TWOBLUECUBES_CATCH_HPP_INCLUDED
        bool operator == (pitch_class const& rhs) const
        { return pc_ == rhs.pc_; }
#endif

        friend bool equal(pitch_class const& lhs, pitch_class const& rhs)
        { return lhs.pc_ == rhs.pc_; }

    private:
        number_type pc_;
    };


    inline pitch_class to_pitch_class(std::string const& s);

    template <std::int8_t MiddleCOctaveNumber = 3>
    struct pitch
    {
        pitch(std::uint8_t pc, std::int8_t pos = MiddleCOctaveNumber)
            : pc_{ pc }
            , octavePos_{ pos }
        {
            constexpr std::int8_t lowestPos = MiddleCOctaveNumber - 5;
            constexpr std::int8_t highestPos = MiddleCOctaveNumber + 5;
            static_assert(lowestPos < highestPos
                            && highestPos - lowestPos == 10);

            assert(pos >= lowestPos && pos <= highestPos);
            // TODO: throw an exception
        }

        pitch(std::string const& pc, std::int8_t pos = MiddleCOctaveNumber)
            : pitch(to_pitch_class(pc), pos)
        { }

#ifndef TWOBLUECUBES_CATCH_HPP_INCLUDED
        template <std::int8_t MiddleCOctaveNumber2>
        bool operator == (pitch<MiddleCOctaveNumber2> const&) const
        { return false; }

        bool operator == (pitch<MiddleCOctaveNumber> const& rhs) const
        { return pc_ == rhs.pc_ && octavePos_ == rhs.octavePos_; }
#endif

        template <std::int8_t MiddleCOctaveNumber1, std::int8_t MiddleCOctaveNumber2>
        friend bool equal(pitch<MiddleCOctaveNumber1> const&, pitch<MiddleCOctaveNumber2> const&)
        { return false; }

        template <std::int8_t MiddleCOctaveNumber1>
        friend bool equal(pitch<MiddleCOctaveNumber1> const& lhs, pitch<MiddleCOctaveNumber1> const& rhs)
        { return equal(lhs.pc_, rhs.pc_) && lhs.octavePos_ == rhs.octavePos_; }

        pitch_class pc_;
        std::int8_t octavePos_;
    };


    inline pitch_class to_pitch_class(std::string const& s);

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
