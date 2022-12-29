#ifndef PJSOMEWORLDCOMPONENT_H
#define PJSOMEWORLDCOMPONENT_H

#include "Base.h"
#include "Updatable.h"
#include <memory>

namespace PJ
{
    class WorldNode;
    
    class SomeWorldComponent : public Base, public Updatable
    {
    protected:
        bool isEnabled = true;

    public:
        std::weak_ptr<WorldNode> owner;

        bool IsEnabled() const { return false; }

        // Called before Start
        virtual void Awake() {}

        // Called after Awake
        virtual void Start() {}

        void OnUpdate(TimeSlice time) override {}
        bool IsFinished() const override { return false; }

        // Called after OnUpdate
        virtual void LateUpdate() {}
    };
}

#endif
