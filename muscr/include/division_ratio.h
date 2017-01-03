#ifndef MUSCR_DIVISION_RATIO_H
#define MUSCR_DIVISION_RATIO_H


#include <stdexcept>


namespace muscr
{
    struct division_ratio
    {
        division_ratio(int num, int denom)
            : numerator_(num)
            , denominator_(denom)
        { }
        
        bool operator < (division_ratio const& rhs) const
        {
            if (0 == denominator_ || 0 == rhs.denominator_) {
                throw std::overflow_error("[muscr::division_ratio] division by zero.");
            }
            return (1.0 * numerator_ / denominator_)
                        < (1.0 * rhs.numerator_ / rhs.denominator_);
        }

        bool operator == (division_ratio const& rhs) const
        {
            return (numerator_ == rhs.numerator_)
                        && (denominator_ == rhs.denominator_);
        }

        int const numerator_;
        int const denominator_; 
    };
} // namespace muscr


#endif // MUSCR_DIVISION_RATIO_H
