#ifndef PJSOMEWORLDCOMPONENT_H
#define PJSOMEWORLDCOMPONENT_H

#include "Base.h"
#include "Updatable.h"
#include "GeoTransform.h"
#include <memory>

namespace PJ
{
    class WorldNode;

    /// Core world component methods
    /// Important: use WorldComponent instead of this for template-convenience methods
    class SomeWorldComponent : public Base, public Updatable
    {
    protected:
        bool isEnabled = true;

    public:
        std::weak_ptr<WorldNode> owner;

        bool IsEnabled() const { return false; }

        void DestroyOwner(float afterSeconds = 0);
        SP<GeoTransform> Transform() const;

        // Called before Start
        virtual void Awake() {}

        // Called after Awake
        virtual void Start() {}

        void OnUpdate(TimeSlice time) override {}
        bool IsFinished() const override { return false; }

        SP<WorldNode> Node() const;

        // Called after OnUpdate
        virtual void LateUpdate() {}
    };
}

#endif
