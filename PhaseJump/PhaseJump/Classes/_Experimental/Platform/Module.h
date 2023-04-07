#ifndef PJMODULE_H
#define PJMODULE_H

#include "Base.h"
#include "Class.h"
#include "ClassRegistry.h"
#include <vector>
#include <memory>
#include <string>

/*
 RATING: 4 stars
 Useful, with unit tests. Could use more features
 CODE REVIEW: 11/10/22
 */
namespace PJ {
    class Module;
    using ModuleSharedPtr = std::shared_ptr<Module>;

    static ClassRegistry classRegistry;

    /// <summary>
    /// Contains tools and code that can be optionally loaded at runtime
    /// Example: audio engine module
    /// </summary>
    class Module : public Base {
    protected:
        ClassRegistry& classRegistry;

        std::vector<ModuleSharedPtr> dependencies;

        virtual void _Go() {}

    public:
        Module(ClassRegistry& classRegistry = PJ::classRegistry) : classRegistry(classRegistry) {
        }

        void Go() override {
            for (auto dependency : dependencies) {
                if (dependency.get()) {
                    dependency->Go();
                }
            }

            _Go();
        }
    };
}

#endif
