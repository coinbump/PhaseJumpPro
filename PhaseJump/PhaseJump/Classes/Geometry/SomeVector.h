#ifndef PJSOMEVECTOR_H
#define PJSOMEVECTOR_H

#include <array>

/*
 RATING: 5 stars
 Utility class with unit tests
 CODE REVIEW: 11/11/22
 */
namespace PJ {
    template <class Math, int dim>
    class SomeVector : public std::array<typename Math::Type, dim>
    {
    public:
        using Base = typename std::array<typename Math::Type, dim>;
        using MathType = typename Math::Type;
        using This = SomeVector<Math, dim>;

        SomeVector() : Base {0} {}

        // Reference: http://farside.ph.utexas.edu/teaching/301/lectures/node28.html
        float Magnitude(bool root = true) const
        {
            float result = 0;
            for (auto component : *this) {
                result += component*component;
            }

            if (root) {
                result = Math::Sqrt(result);
            }
            return result;
        }

        void Normalize()
        {
            float mag = Magnitude(true);
            for (int i = 0; i < dim; i++) {
                (*this)[i] = (*this)[i]/mag;
            }
        }

        // Reference: http://rosettacode.org/wiki/Dot_product#C
        MathType DotProduct(This const& b) const
        {
            float result = 0;
            for (int i = 0; i < dim; i++) {
                result += (*this)[i]*b[i];
            }
            return result;
        }
    };
}

#endif
