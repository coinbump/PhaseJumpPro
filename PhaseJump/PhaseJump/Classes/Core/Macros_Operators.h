#ifndef PJMACROS_OPERATORS_H
#define PJMACROS_OPERATORS_H

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

#endif
