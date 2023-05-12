#ifndef PJSOMEWORLDCOMPONENT_H
#define PJSOMEWORLDCOMPONENT_H

#include "Base.h"
#include "Updatable.h"
#include "GeoTransform.h"
#include "WorldNodeTransform.h"
#include <memory>

namespace PJ
{
    class World;
    class WorldNode;

    /// Core world component methods
    /// Important: use WorldComponent instead of this for template-convenience methods
    class SomeWorldComponent : public Base, public Updatable
    {
    protected:
        bool isEnabled = true;

    public:
        using NodeTransform = WorldNodeTransform;

        WP<WorldNode> owner;

        bool IsEnabled() const { return isEnabled; }

        void DestroyOwner(float afterSeconds = 0);
        SP<NodeTransform> Transform() const;

        // Called before Start
        virtual void Awake() {}

        // Called after Awake
        virtual void Start() {}

        void OnUpdate(TimeSlice time) override {}
        bool IsFinished() const override { return false; }

        SP<WorldNode> Node() const;

        // Called after OnUpdate
        virtual void LateUpdate() {}

        SP<World> World() const;
    };
}

#endif
