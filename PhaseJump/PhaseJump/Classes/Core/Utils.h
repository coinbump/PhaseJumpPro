#pragma once

#include "Macros.h"
#include <functional>
#include <memory>

/// MARK: - Pointer cast convenience

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// Casts source to dest type
    template <class Dest, class Source>
    Dest* As(Source* source) {
        return dynamic_cast<Dest*>(source);
    }

    /// Casts source to dest type
    template <class Dest, class Source>
    Dest const* As(Source const* source) {
        return dynamic_cast<Dest const*>(source);
    }

    /// @return Returns true if source type is dest type or a subclass
    template <class Dest, class Source>
    bool Is(Source* source) {
        return nullptr != dynamic_cast<Dest*>(source);
    }

    /// @return Returns true if source type is dest type or a subclass
    template <class Dest, class Source>
    bool Is(Source const* source) {
        return nullptr != dynamic_cast<Dest const*>(source);
    }

    /// @return Returns true if source type is dest type or a subclass
    template <class Dest, class Source>
    bool Is(Source const& source) {
        return nullptr != dynamic_cast<Dest const*>(&source);
    }

    constexpr bool Toggled(bool value) {
        return !value;
    }

    constexpr void Toggle(bool& value) {
        value = !value;
    }

    template <class Type>
    Type Lerp(Type start, Type end, float factor) {
        return start + ((end - start) * factor);
    }
} // namespace PJ

/// MARK: - Shared pointer aliases

/**
 Use aliases for shared pointers to have the flexibility of introducing a faster
 memory model in the future
 */
template <class T>
using SP = std::shared_ptr<T>;

template <class T>
using UP = std::unique_ptr<T>;

template <class T>
using SPC = std::shared_ptr<T> const&;

template <class T>
using WP = std::weak_ptr<T>;

template <class T>
SP<T> LOCK(WP<T> const& value) {
    return value.lock();
}

/// MARK: - Shared Pointer cast convenience

// WARNING: This produces the warning: stack nearly exhausted; compilation time may suffer, and
// crashes due to stack overflow are likely
// template <class Type, typename... Args>
// auto NEW(Args&&... args) -> decltype(NEW<Type>(std::forward<Args>(args)...)) {
//    return NEW<Type>(std::forward<Args>(args)...);
//}

template <class _Tp, class... _Args>
inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX23
    typename std::__unique_if<_Tp>::__unique_single
    NEW(_Args&&... __args) {
    return std::unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}

#ifdef _WIN32

template <class _Tp, class _Up>
std::shared_ptr<_Tp> SCAST(const std::shared_ptr<_Up>& __r) {
    return std::static_pointer_cast<_Tp>(__r);
}

template <class _Tp, class... _Args, class = std::enable_if_t<!std::is_array<_Tp>::value>>
std::shared_ptr<_Tp> MAKE(_Args&&... __args) {
    return std::make_shared<_Tp>(std::forward<_Args>(__args)...);
}

template <class _Tp, class _Up>
std::shared_ptr<_Tp> DCAST(const std::shared_ptr<_Up>& __r) {
    return std::dynamic_pointer_cast<_Tp>(__r);
}

#else

template <class _Tp, class... _Args, class = std::__enable_if_t<!std::is_array<_Tp>::value>>
std::shared_ptr<_Tp> MAKE(_Args&&... __args) {
    return std::make_shared<_Tp>(std::forward<_Args>(__args)...);
}

template <class _Tp, class _Up>
std::shared_ptr<_Tp> SCAST(const std::shared_ptr<_Up>& __r) _NOEXCEPT {
    return std::static_pointer_cast<_Tp>(__r);
}

template <class _Tp, class _Up>
std::shared_ptr<_Tp> DCAST(const std::shared_ptr<_Up>& __r) _NOEXCEPT {
    return std::dynamic_pointer_cast<_Tp>(__r);
}

#endif
