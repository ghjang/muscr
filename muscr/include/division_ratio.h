#ifndef MUSCR_DIVISION_RATIO_H
#define MUSCR_DIVISION_RATIO_H


#include <stdexcept>


namespace muscr::division_ratio::detail
{
    template <typename T>
    auto gcd_impl(T a, T b)
    {
        if (0 == b) {
            return a;
        }
        return gcd_impl(b, a % b);
    }

    template <typename T>
    int gcd(T a, T b)
    {
        if (a == 0 || b == 0) {
            return 0;
        }
        return gcd_impl(a, b);
    }
} // muscr::division_ratio::detail

namespace muscr::division_ratio
{
    template <int Factor = 1>
    struct ratio
    {
    public:
        ratio(int num = 0, int denom = 1)
            : numerator_(num)
            , denominator_(denom)
        { }

    public:    
        bool operator < (ratio<Factor> const& rhs) const
        {
            if (0 == denominator_ || 0 == rhs.denominator_) {
                throw std::overflow_error("[muscr::division_ratio] division by zero.");
            }
            return ((1.0 * Factor * numerator_) / (Factor * denominator_))
                        < ((1.0 * Factor * rhs.numerator_) / (Factor * rhs.denominator_));
        }

        bool operator == (ratio<Factor> const& rhs) const
        {
            return (Factor * numerator_ == Factor * rhs.numerator_)
                        && (Factor * denominator_ == Factor * rhs.denominator_);
        }

    public:
        ratio<Factor> operator * (ratio<Factor> const& rhs) const
        {
            return ratio<Factor>{ numerator_ * rhs.numerator_, denominator_ * rhs.denominator_ };
        }

    public:
        ratio<Factor> to_lowest_term() const
        {
            auto gcd = detail::gcd(numerator_, denominator_);
            return ratio<Factor>{ numerator_ / gcd, denominator_ / gcd };
        }
    
    public:
        int const numerator_;
        int const denominator_; 
    };

    ratio<> const whole_note        { 4, 1 };
    ratio<> const half_note         { 2, 1 };
    ratio<> const quater_note       { 1, 1 };
    ratio<> const eighth_note       { 1, 2 };
    ratio<> const sixteenth_note    { 1, 4 };

    ratio<10> const whole_rest      { 4, 1 };
    ratio<10> const half_rest       { 2, 1 };
    ratio<10> const quater_rest     { 1, 1 };
    ratio<10> const eighth_rest     { 1, 2 };
    ratio<10> const sixteenth_rest  { 1, 4 };
} // namespace muscr::division_ratio


#endif // MUSCR_DIVISION_RATIO_H
