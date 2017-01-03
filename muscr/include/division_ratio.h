#ifndef MUSCR_DIVISION_RATIO_H
#define MUSCR_DIVISION_RATIO_H


#include <stdexcept>


namespace muscr::division_ratio
{
    struct ratio
    {
        ratio(int num, int denom)
            : numerator_(num)
            , denominator_(denom)
        { }
        
        bool operator < (ratio const& rhs) const
        {
            if (0 == denominator_ || 0 == rhs.denominator_) {
                throw std::overflow_error("[muscr::division_ratio] division by zero.");
            }
            return (1.0 * numerator_ / denominator_)
                        < (1.0 * rhs.numerator_ / rhs.denominator_);
        }

        bool operator == (ratio const& rhs) const
        {
            return (numerator_ == rhs.numerator_)
                        && (denominator_ == rhs.denominator_);
        }

        int const numerator_;
        int const denominator_; 
    };

    ratio const whole_note{ 4, 1 };
    ratio const half_note{ 2, 1 };
    ratio const quater_note{ 1, 1 };
    ratio const eighth_note{ 1, 2 };
    ratio const sixteenth_note{ 1, 4 };

    ratio const whole_rest{ 4, 1 };
    ratio const half_rest{ 2, 1 };
    ratio const quater_rest{ 1, 1 };
    ratio const eighth_rest{ 1, 2 };
    ratio const sixteenth_rest{ 1, 4 };
} // namespace muscr::division_ratio


#endif // MUSCR_DIVISION_RATIO_H
