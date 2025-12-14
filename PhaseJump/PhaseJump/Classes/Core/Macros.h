#pragma once

#include <iostream>

// MARK: - Template Specialization

/// Enable a function if Type is the same as CheckType
// TODO: use std::requires
#define SPECIALIZE_IS_BASE_OF(TemplateType, BaseType) \
    template <                                        \
        typename _Type = TemplateType,                \
        std::enable_if_t<std::is_base_of<BaseType, _Type>::value>* = nullptr>

#define SPECIALIZE_IS_SAME(TemplateType, CheckType) \
    template <                                      \
        typename _Type = TemplateType,              \
        std::enable_if_t<std::is_same<_Type, CheckType>::value>* = nullptr>

#define DELETE_COPY(Type)       \
    Type(Type const&) = delete; \
    Type& operator=(Type const&) = delete;

// TODO: add unit tests
#define GUARD(test) \
    if (!(test)) {  \
        return;     \
    }

#define GUARD_THROW(test, exception) \
    if (!(test)) {                   \
        throw exception;             \
    }

#define GUARDR(test, result) \
    if (!(test)) {           \
        return result;       \
    }

#define GUARD_CONTINUE(test) \
    if (!(test)) {           \
        continue;            \
    }

#define GUARD_BREAK(test) \
    if (!(test)) {        \
        break;            \
    }

// TODO: re-evaluate these
// #define STR(a) #a
//
///// Adds getter and setters for value
// #def ine  GET_SET(value, ValueType, ValueFunc)            \
//    ValueType const& ValueFunc() const {                \
//        return value;                                   \
//    }                                                   \
//    void SetSTR(ValueFunc)(ValueType const& newValue) { \
//        this->value = newValue;                         \
//    }
