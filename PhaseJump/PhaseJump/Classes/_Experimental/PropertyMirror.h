#pragma once

#include "Binding.h"
#include "VectorList.h"
#include <functional>

// Experimental
// TODO: see mirror improvements made in scrap code
namespace PJ {
    /// Added to objects via mirror macros to allow reflection
    /// Example: visual inspector to edit properties
    class SomePropertyMirror {
    public:
        virtual ~SomePropertyMirror() {}

        /// User facing name
        String name;

        SomePropertyMirror(String name) :
            name(name) {}
    };

    /// Collection of property mirrors for reflection
    class PropertyMirrors {
    public:
        using MirrorList = VectorList<SomePropertyMirror*>;

        /// User facing name
        String name;

        MirrorList value;

        PropertyMirrors(String name) :
            name(name) {}

        void Add(SomePropertyMirror* propertyMirror) {
            value.push_back(propertyMirror);
        }

        size_t Count() const {
            return value.size();
        }
    };

    /// Reflects a property
    template <class T>
    class PropertyMirror : public SomePropertyMirror {
    public:
        using Base = SomePropertyMirror;
        using This = PropertyMirror<T>;
        using GetFunc = std::function<T const&()>;
        using SetFunc = std::function<void(T)>;

        GetFunc getFunc;
        SetFunc setFunc;

        PropertyMirror(PropertyMirrors& container, String name, GetFunc getFunc, SetFunc setFunc) :
            Base(name),
            getFunc(getFunc),
            setFunc(setFunc) {
            container.Add(this);
        }

        operator T const&() {
            return getFunc();
        }

        This& operator=(T rhs) {
            setFunc(rhs);
            return *this;
        }
    };

    /// Reflects a child object that also supports reflection
    class ChildPropertyMirror : public SomePropertyMirror {
    public:
        using Base = SomePropertyMirror;
        using MirrorsFunc = std::function<PropertyMirrors*()>;

        MirrorsFunc mirrorsFunc;

        ChildPropertyMirror(PropertyMirrors& container, String name, MirrorsFunc mirrorsFunc) :
            Base(name),
            mirrorsFunc(mirrorsFunc) {
            container.Add(this);
        }

        operator PropertyMirrors*() {
            return mirrorsFunc();
        }
    };
} // namespace PJ

// Checklist: disable mirrors for final release build
#ifdef _DISABLE_MIRRORS_
#define ENABLE_MIRRORS(userName)

#define MIRROR(Type, name, userName) Type name{};

#define MIRROR_CHILD(Type, name, userName) Type name;

#define MIRROR_CHILD_PTR(Type, name, userName) Type name;

#else
/// Declare inside class/struct to enable mirror reflection of properties
#define ENABLE_MIRRORS(userName) PropertyMirrors _mirrors{ userName };

/// Declare for each property that is mirrored
#define MIRROR(Type, name)                                                               \
    Type name{};                                                                         \
    PropertyMirror<Type> name##Mirror{ _mirrors, #name, [this]() { return this->name; }, \
                                       [this](Type value) { this->name = value; } };

/// Declare for value-type child objects that are mirrored
#define MIRROR_CHILD(Type, name) \
    Type name;                   \
    ChildPropertyMirror name##Mirror{ _mirrors, #name, [this]() { return &this->name._mirrors; } };

/// Declare for pointer-type child objects that are mirrored
#define MIRROR_CHILD_PTR(Type, name) \
    Type name;                       \
    ChildPropertyMirror name##Mirror{ _mirrors, #name, [this]() { return &this->name->_mirrors; } };

#endif
