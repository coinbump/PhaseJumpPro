#ifndef PJPACKAGE_h
#define PJPACKAGE_h

#include "Base.h"
#include "Module.h"
#include <vector>
#include <memory>

namespace PJ {
    /// <summary>
    /// A collection of modules
    /// </summary>
    class Package : public Base {
    public:
        std::vector<ModuleSharedPtr> modules;

        void Go() override {
            for (auto _module : modules) {
                if (_module.get()) {
                    _module->Go();
                }
            }
        }
    };
}

#endif
