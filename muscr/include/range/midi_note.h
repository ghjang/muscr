#ifndef MUSCR_MIDI_NOTE_H
#define MUSCR_MIDI_NOTE_H


#include <cassert>
#include <cstdint>

#include <range/v3/all.hpp>


// refer to http://www.electronics.dit.ie/staff/tscarff/Music_technology/midi/midi_note_numbers_for_octaves.htm


namespace muscr::range
{
    struct midi_note_number
    {
        using number_type = std::uint8_t;

        operator number_type () const
        { return n_; }

        number_type const n_;
    };
    
    struct midi_note_number_view
            : ranges::view_facade<midi_note_number_view>
    {
    public:
        using number_type = std::uint8_t;

    public:
        midi_note_number_view(number_type start = 0, number_type end = 127)
            : start_{ start }
            , end_{ end }
        {
            assert(start >= 0 && start <= 127);
            assert(end >= 0 && end <= 127);
            assert(start <= end);

            // TODO: throw an exception.
        }

    private:
        number_type const start_;
        number_type const end_;

    private:
        friend struct ranges::range_access;

        struct cursor
        {
            midi_note_number get() const
            { return midi_note_number{ n_ }; }

            bool equal(cursor const& rhs) const
            { return n_ == rhs.n_; }

            void next()
            { ++n_; }

            void prev()
            { --n_; }

            std::ptrdiff_t distance_to(cursor const& rhs) const
            { return rhs.n_ - n_; }

            void advance(std::ptrdiff_t n)
            { n_ += n; }

            number_type n_;   
        };

        cursor begin_cursor() const
        {
            assert(start_ <= end_);
            // TODO: throw an exception.

            return { start_ };
        }

        cursor end_cursor() const
        {
            assert(start_ <= end_);
            // TODO: throw an exception.

            return { static_cast<number_type>(end_ + 1) };
        }
    };
} // namespace muscr::range


#endif // MUSCR_MIDI_NOTE_H
