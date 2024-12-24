#pragma once

#include <math.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/20/24
 */
/// Standard vector operators
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
