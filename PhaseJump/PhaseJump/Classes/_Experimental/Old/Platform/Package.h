#pragma once

#include "Base.h"
#include "Module.h"
#include "VectorList.h"
#include <memory>
#include <vector>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    /// A collection of modules
    class Package : public Base {
    public:
        VectorList<ModuleSharedPtr> modules;

    protected:
        // MARK: Base

        void OnGo() override {
            Base::OnGo();

            for (auto& _module : modules) {
                if (_module.get()) {
                    _module->Go();
                }
            }
        }
    };
} // namespace PJ
