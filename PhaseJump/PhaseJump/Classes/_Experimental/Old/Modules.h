#pragma once

#include "ClassId.h"
#include "Module.h"
#include "TypeClass.h"

/*
 RATING: 4 stars
 Some unit tests, limited functionality
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Demonstration class for modules (not a real class)
    class _Foo : public Base {
    public:
        using RootBaseType = Base;

        _Foo() {}

        virtual ~_Foo() {}
    };

    /// Demonstration class for modules (not a real class)
    class _MacFoo : public _Foo {
    public:
        _MacFoo() {}
    };

    /// Demonstration class for modules (not a real class)
    class _FooClass : public TypeClass<Base> {
    public:
        using Base = TypeClass<PJ::Base>;

        _FooClass() :
            Base(ClassId::Foo, []() { return NEW<_Foo>(); }) {}

        virtual ~_FooClass() {}
    };

    // Demonstration class for modules (not a real class)
    class _MacFooClass : public TypeClass<Base> {
    public:
        using Base = TypeClass<PJ::Base>;

        _MacFooClass() :
            Base(ClassId::Foo, []() { return NEW<_MacFoo>(); }) {}
    };

    /// Registers class objects for core types (platform neutral)
    class CoreModule : public Module {
    protected:
        using Base = Module;

        void Configure() override {
            classRegistry.Add(ClassId::Foo, NEW<_FooClass>());
        }

    public:
        CoreModule(ClassRegistry& classRegistry) :
            Module(classRegistry) {}
    };

    /// Registers class objects for Mac platform
    class MacPlatformModule : public Module {
    protected:
        using Base = Module;

        void Configure() override {
            classRegistry.Add(ClassId::Foo, NEW<_MacFooClass>());
        }

    public:
        MacPlatformModule(ClassRegistry& classRegistry) :
            Module(classRegistry) {
            dependencies.push_back(MAKE<CoreModule>(classRegistry));
        }
    };
} // namespace PJ
