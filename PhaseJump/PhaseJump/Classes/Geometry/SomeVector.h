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
        typedef typename std::array<typename Math::Type, dim> Base;
        typedef typename Math::Type MathType;
        typedef SomeVector<Math, dim> This;

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

        This operator+(This const& b) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] += b[i];
            }
            return result;
        }

        This& operator+=(This const& b)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] += b[i];
            }
            return *this;
        }

        This operator-(This const& b) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] -= b[i];
            }
            return result;
        }

        This& operator-=(This const& b)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] -= b[i];
            }
            return *this;
        }

        This operator*(MathType mult) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] *= mult;
            }
            return result;
        }

        This operator*(This const& b) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] *= b[i];
            }
            return result;
        }

        This& operator*=(This const& b)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] *= b[i];
            }
            return *this;
        }

        This& operator*=(MathType scale)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] *= scale;
            }
            return *this;
        }

        This operator/(MathType div) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] /= div;
            }
            return result;
        }

        This operator/(This const& b) const
        {
            This result = *this;
            for (int i = 0; i < dim; i++) {
                result[i] /= b[i];
            }
            return result;
        }

        This& operator/=(This const& b)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] /= b[i];
            }
            return *this;
        }

        This& operator/=(MathType scale)
        {
            for (int i = 0; i < dim; i++) {
                (*this)[i] /= scale;
            }
            return *this;
        }
    };
}

#endif
