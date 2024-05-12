#ifndef PJUISYSTEMSHAREDDEPENDENCYRESOLVER_H
#define PJUISYSTEMSHAREDDEPENDENCYRESOLVER_H

#include "SomeDependencyResolver.h"
#include "UISystem.h"

namespace PJ {
    class UISystemSharedDependencyResolver : public SomeDependencyResolver<SP<UISystem>>
    {
    public:
        UISystemSharedDependencyResolver() {
        }
        virtual ~UISystemSharedDependencyResolver() {}

        SP<UISystem> Dependency() override {
            return UISystem::shared;
        }
    };
}

#endif
