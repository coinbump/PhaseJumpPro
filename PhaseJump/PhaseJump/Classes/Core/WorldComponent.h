#ifndef PJWORLDCOMPONENT_H
#define PJWORLDCOMPONENT_H

#include "AcyclicGraphNode.h"
#include "Updatable.h"
#include <memory>

namespace PJ {
    class WorldNode;

    /// <summary>
    /// A component that can be attached to world nodes
    /// </summary>
    class WorldComponent : public Updatable {
    protected:
        bool isEnabled = true;

    public:
        std::weak_ptr<WorldNode> owner;

        bool IsEnabled() const { return false; }

        // Called before Start
        virtual void Awake() {}

        // Called after Awake
        virtual void Start() {}

        void DestroyOwner(float afterSeconds = 0);
    };
}

#endif
