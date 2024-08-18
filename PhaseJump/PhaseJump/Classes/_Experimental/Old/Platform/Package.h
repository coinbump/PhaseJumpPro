#ifndef PJPACKAGE_h
#define PJPACKAGE_h

#include "Base.h"
#include "Module.h"
#include <memory>
#include <vector>

// CODE REVIEW: ?/23
namespace PJ {
    /// A collection of modules
    class Package : public Base {
    public:
        std::vector<ModuleSharedPtr> modules;

        void GoInternal() override {
            for (auto& _module : modules) {
                if (_module.get()) {
                    _module->Go();
                }
            }
        }
    };
} // namespace PJ

#endif
