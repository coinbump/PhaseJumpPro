#pragma once

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/20/24
 */
/// Add common math operators based on a single value
#define MATH_OPERATORS(Type, Value)       \
    Type operator+(Type const& b) const { \
        return Type(Value + b.Value);     \
    }                                     \
    Type operator-(Type const& b) const { \
        return Type(Value - b.Value);     \
    }                                     \
    Type operator*(Type const& b) const { \
        return Type(Value * b.Value);     \
    }                                     \
    Type operator/(Type const& b) const { \
        return Type(Value / b.Value);     \
    }                                     \
    Type& operator+=(Type const& b) {     \
        this->Value += b.Value;           \
        return *this;                     \
    }                                     \
    Type& operator-=(Type const& b) {     \
        this->Value -= b.Value;           \
        return *this;                     \
    }                                     \
    Type& operator/=(Type const& b) {     \
        this->Value /= b.Value;           \
        return *this;                     \
    }                                     \
    Type& operator*=(Type const& b) {     \
        this->Value *= b.Value;           \
        return *this;                     \
    }
