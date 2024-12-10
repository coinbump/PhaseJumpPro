#ifndef PJMODULE_H
#define PJMODULE_H

#include "Base.h"
#include "Class.h"
#include "ClassRegistry.h"
#include "List.h"
#include "StringUtils.h"
#include <memory>

/*
 RATING: 4 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    class Module;
    using ModuleSharedPtr = SP<Module>;

    /// Global class registry
    static ClassRegistry classRegistry;

    /// A module registers class objects on Go(). Each class object contains a
    /// factory for a specific type The class object is retrieved from the class
    /// registry, then the factory is used to create the registered
    /// implementation This allows us to register classes based on platform.
    /// (Example: MacTypeClass vs WinTypeClass) Each module can have N
    /// dependencies, each of which can register its own class objects Example:
    /// both iOSModule and MacModule depend on NSModule (NextStep is the
    /// foundation of both)
    class Module : public Base {
    protected:
        ClassRegistry<>& classRegistry;

        List<ModuleSharedPtr> dependencies;

        virtual void Configure() = 0;

    public:
        Module(ClassRegistry<>& classRegistry = PJ::classRegistry) :
            classRegistry(classRegistry) {}

        virtual ~Module() {}

        // MARK: Base

        void OnGo() override {
            for (auto& dependency : dependencies) {
                if (dependency.get()) {
                    dependency->Go();
                }
            }

            Configure();
        }
    };
} // namespace PJ

#endif
