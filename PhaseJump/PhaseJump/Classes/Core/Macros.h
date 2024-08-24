#pragma once

// MARK: - Template Specialization

/// Enable a function if Type is the same as CheckType
#define SPECIALIZE_IS_BASE_OF(TemplateType, BaseType) \
    template <                                        \
        typename _Type = TemplateType,                \
        std::enable_if_t<std::is_base_of<BaseType, _Type>::value>* = nullptr>

#define SPECIALIZE_IS_SAME(TemplateType, CheckType) \
    template <                                      \
        typename _Type = TemplateType,              \
        std::enable_if_t<std::is_same<_Type, CheckType>::value>* = nullptr>

// TODO: add unit tests
#define GUARD(test) \
    if (!(test)) {  \
        return;     \
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
