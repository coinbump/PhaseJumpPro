#ifndef PJMACROS_H
#define PJMACROS_H

#include <memory>

// Convenience types
template <class T>
using SP = std::shared_ptr<T>;

template <class T>
using SPC = std::shared_ptr<T> const&;

template <class T>
using WP = std::weak_ptr<T>;

template<class _Tp, class ..._Args, class = std::__enable_if_t<!std::is_array<_Tp>::value>>
std::shared_ptr<_Tp> MAKE(_Args&& ...__args)
{
    return std::make_shared<_Tp>(std::forward<_Args>(__args)...);
}

template<class _Tp, class _Up>
std::shared_ptr<_Tp>
SCAST(const std::shared_ptr<_Up>& __r) _NOEXCEPT
{
    return static_pointer_cast<_Tp>(__r);
}

template<class _Tp, class _Up>
std::shared_ptr<_Tp>
DCAST(const std::shared_ptr<_Up>& __r) _NOEXCEPT
{
    return dynamic_pointer_cast<_Tp>(__r);
}

#endif
