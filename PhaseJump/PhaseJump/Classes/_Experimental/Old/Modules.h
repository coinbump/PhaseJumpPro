#pragma once

#include "ClassId.h"
#include "Module.h"
#include "TypeClass.h"

/*
 RATING: 4 stars
 Useful, with unit tests. Could use more features
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    // Demonstration class for modules (not a real class)
    class _Foo : public Base {
    public:
        using RootBaseType = Base;

        _Foo() {}

        virtual ~_Foo() {}
    };

    // Demonstration class for modules (not a real class)
    class _MacFoo : public _Foo {
    public:
        _MacFoo() {}
    };

    // Demonstration class for modules (not a real class)
    class _FooClass : public TypeClass<_Foo> {
    public:
        _FooClass() :
            TypeClass<_Foo>(ClassId::Foo, []() { return MAKE<_Foo>(); }) {}

        virtual ~_FooClass() {}
    };

    // Demonstration class for modules (not a real class)
    class _MacFooClass : public TypeClass<_MacFoo> {
    public:
        _MacFooClass() :
            TypeClass<_MacFoo>(ClassId::Foo, []() { return MAKE<_MacFoo>(); }) {}
    };

    /// Registers class objects for core types (platform neutral)
    class CoreModule : public Module {
    protected:
        using Base = Module;

        void Configure() override {
            classRegistry.map[ClassId::Foo] = MAKE<_FooClass>();
        }

    public:
        CoreModule(ClassRegistry<>& classRegistry = PJ::classRegistry) :
            Module(classRegistry) {}
    };

    /// Registers class objects for Mac platform
    class MacPlatformModule : public Module {
    protected:
        using Base = Module;

        void Configure() override {
            classRegistry.map[ClassId::Foo] = MAKE<_MacFooClass>();
        }

    public:
        MacPlatformModule(ClassRegistry<>& classRegistry = PJ::classRegistry) :
            Module(classRegistry) {
            dependencies.push_back(ModuleSharedPtr(new CoreModule(classRegistry)));
        }
    };
} // namespace PJ
