#ifndef PJMACROSVECTORS_H
#define PJMACROSVECTORS_H

#include <math.h>

// TODO: check that these are covered by Vector2 unit tests
// Used so that the methods return the vector type, not the parent class
#define VECTOR_METHODS(Vector, MathType, dim)                           \
    Vector operator+(Vector const& b) const {                           \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] += b[i];                                          \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector& operator+=(Vector const& b) {                               \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] += b[i];                                         \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    Vector operator-(Vector const& b) const {                           \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] -= b[i];                                          \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector& operator-=(Vector const& b) {                               \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] -= b[i];                                         \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    Vector operator*(MathType mult) const {                             \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] *= mult;                                          \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector operator*(Vector const& b) const {                           \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] *= b[i];                                          \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector& operator*=(Vector const& b) {                               \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] *= b[i];                                         \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    Vector& operator*=(MathType scale) {                                \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] *= scale;                                        \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    Vector operator/(MathType div) const {                              \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] /= div;                                           \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector operator/(Vector const& b) const {                           \
        Vector result = *this;                                          \
        for (int i = 0; i < dim; i++) {                                 \
            result[i] /= b[i];                                          \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    Vector& operator/=(Vector const& b) {                               \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] /= b[i];                                         \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
                                                                        \
    Vector& operator/=(MathType scale) {                                \
        for (int i = 0; i < dim; i++) {                                 \
            (*this)[i] /= scale;                                        \
        }                                                               \
        return *this;                                                   \
    }                                                                   \
    friend Vector operator*(MathType lhs, Vector rhs) {                 \
        return rhs * lhs;                                               \
    }                                                                   \
                                                                        \
    /* Reference:                                                       \
     * http://farside.ph.utexas.edu/teaching/301/lectures/node28.html*/ \
    float Magnitude(bool root = true) const {                           \
        float result = 0;                                               \
        for (int i = 0; i < dim; i++) {                                 \
            auto component = (float)(*this)[i];                         \
            result += component * component;                            \
        }                                                               \
                                                                        \
        if (root) {                                                     \
            result = sqrtf(result);                                     \
        }                                                               \
        return result;                                                  \
    }                                                                   \
    /* Reference: http://rosettacode.org/wiki/Dot_product#C */          \
    float DotProduct(Vector const& b) const {                           \
        float result = 0;                                               \
        for (int i = 0; i < dim; i++) {                                 \
            result += (float)(*this)[i] * (float)b[i];                  \
        }                                                               \
        return result;                                                  \
    }

#define VECTOR_EXTERNALMETHODS(Vector, MathType) //

#endif
