#ifndef PJMACROSVECTORS_H
#define PJMACROSVECTORS_H

// Used so that the methods return the vector type, not the parent class
#define VECTOR_METHODS(Vector, dim) \
Vector operator+(Vector const& b) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] += b[i]; \
    } \
    return result; \
} \
\
Vector& operator+=(Vector const& b) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] += b[i]; \
    } \
    return *this; \
} \
\
Vector operator-(Vector const& b) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] -= b[i]; \
    } \
    return result; \
} \
\
Vector& operator-=(Vector const& b) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] -= b[i]; \
    } \
    return *this; \
} \
\
Vector operator*(MathType mult) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] *= mult; \
    } \
    return result; \
} \
\
Vector operator*(Vector const& b) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] *= b[i]; \
    } \
    return result; \
} \
\
Vector& operator*=(Vector const& b) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] *= b[i]; \
    } \
    return *this; \
} \
\
Vector& operator*=(MathType scale) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] *= scale; \
    } \
    return *this; \
} \
\
Vector operator/(MathType div) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] /= div; \
    } \
    return result; \
} \
\
Vector operator/(Vector const& b) const \
{ \
    Vector result = *this; \
    for (int i = 0; i < dim; i++) { \
        result[i] /= b[i]; \
    } \
    return result; \
} \
\
Vector& operator/=(Vector const& b) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] /= b[i]; \
    } \
    return *this; \
} \
\
Vector& operator/=(MathType scale) \
{ \
    for (int i = 0; i < dim; i++) { \
        (*this)[i] /= scale; \
    } \
    return *this; \
} \
friend Vector operator*(MathType lhs, Vector rhs) \
{ \
    return rhs * lhs; \
} \

#define VECTOR_EXTERNALMETHODS(Vector, MathType) //

#endif
