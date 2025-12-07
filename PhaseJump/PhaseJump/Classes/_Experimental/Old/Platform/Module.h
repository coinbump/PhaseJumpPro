#pragma once

#include "Base.h"
#include "Class.h"
#include "ClassRegistry.h"
#include "List.h"
#include "StringUtils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    class Module;
    using ModuleSharedPtr = SP<Module>;

    /// Global class registry
    static ClassRegistry classRegistry;

    /**
     A module registers class objects when it starts. Each class object contains a factory for a
     specific type. The class object is retrieved from the class registry, then the factory is used
     to create the registered implementation.

     This allows us to register classes based on platform. (Example: MacTypeClass vs WinTypeClass)

     Each module can have N dependencies, each of which can register its own class objects Example:
     both iOSModule and MacModule depend on NSModule (NextStep is the foundation of both)
     */
    class Module : public Base {
    public:
        using This = Module;
        using ClassRegistry = PJ::ClassRegistry<TypeClass<Base>>;

        using ConfigureFunc = std::function<void(This&)>;

    protected:
        ClassRegistry& classRegistry;

        VectorList<ModuleSharedPtr> dependencies;

        virtual void Configure() {
            GUARD(configureFunc)
            configureFunc(*this);
        }

    public:
        ConfigureFunc configureFunc;

        Module(ClassRegistry& classRegistry) :
            classRegistry(classRegistry) {}

        virtual ~Module() {}

        // MARK: Base

        void OnGo() override {
            Base::OnGo();

            // Dependency classes are registered first, then the parent module
            // can override them
            for (auto& dependency : dependencies) {
                if (dependency.get()) {
                    dependency->Go();
                }
            }

            Configure();
        }
    };
} // namespace PJ
