#pragma once

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/3/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Use CRTP to provide a unique String identifier
    /// Note: CRTP is not generally useful for methods we expect to be overriden in subclasses
    /// FUTURE: evaluate deduced this pattern for CRTP with C++ 23
    template <class Derived>
    class StringIdIdentifiable {
    public:
        String StringId() const {
            return static_cast<Derived const*>(this)->StringIdImpl();
        }
    };

    /// Use CRTP to provide a unique 64-bit int identifier
    /// Note: CRTP is not generally useful for methods we expect to be overriden in subclasses
    /// FUTURE: evaluate deduced this pattern for CRTP with C++ 23
    template <class Derived>
    class IntIdentifiable {
    public:
        uint64_t IntId() const {
            return static_cast<Derived const*>(this)->IntIdImpl();
        }
    };
} // namespace PJ
