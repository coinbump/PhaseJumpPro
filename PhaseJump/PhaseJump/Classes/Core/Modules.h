#ifndef PJMODULES_H
#define PJMODULES_H

#include "Module.h"
#include "ClassIds.h"

/*
 RATING: 4 stars
 Useful, with unit tests. Could use more features
 CODE REVIEW: 11/10/22
 */
namespace PJ {
    // Demonstration class for modules (not a real class)
    class _Foo : public Base {
    public:
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
        _FooClass() : TypeClass<_Foo>(ClassIds::foo) {}
        virtual ~_FooClass() {}
    };

    // Demonstration class for modules (not a real class)
    class _MacFooClass : public TypeClass<_MacFoo> {
    public:
        _MacFooClass() : TypeClass<_MacFoo>(ClassIds::foo) {}
    };

    namespace Modules {
        /// <summary>
        /// Core classes, platform-neutral
        /// </summary>
        class Core : public Module {
        protected:
            using Base = Module;

            void _Go() override {
                Base::_Go();

                classRegistry[ClassIds::foo] = std::make_shared<_FooClass>();
            }

        public:
            Core(ClassRegistry& classRegistry = PJ::classRegistry) : Module(classRegistry) {
            }
        };

        /// <summary>
        /// Mac classes
        /// </summary>
        class MacPlatform : public Module {
        protected:
            using Base = Module;

            void _Go() override {
                Base::_Go();

                classRegistry[ClassIds::foo] = std::make_shared<_MacFooClass>();
            }

        public:
            MacPlatform(ClassRegistry& classRegistry = PJ::classRegistry) : Module(classRegistry) {
                dependencies.push_back(ModuleSharedPtr(new Core(classRegistry)));
            }
        };
    }
}

#endif
