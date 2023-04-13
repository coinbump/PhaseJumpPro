#ifndef PJWORLDCOMPONENT_H
#define PJWORLDCOMPONENT_H

#include "SomeWorldComponent.h"
#include "WorldNode.h"
#include "AcyclicGraphNode.h"
#include "GeoTransform.h"
#include "Updatable.h"
#include <memory>

namespace PJ {
    class WorldNode;

    /// <summary>
    /// A component that can be attached to world nodes
    /// </summary>
    class WorldComponent : public SomeWorldComponent {
    public:
        std::shared_ptr<GeoTransform> Transform() const;

        template <class T>
        std::shared_ptr<T> TypeComponent() const
        {
            if (owner.expired()) { return nullptr; }
            return owner.lock()->TypeComponent<T>();
        }

        void DestroyOwner(float afterSeconds = 0);

        VectorList<std::shared_ptr<WorldNode>> ChildNodes() const
        {
            if (owner.expired()) { return VectorList<std::shared_ptr<WorldNode>>(); }
            return owner.lock()->ChildNodes();
        }

        // TODO: pull in code from WorldComponentBeta

        // Convenience names
        template <class T>
        std::shared_ptr<T> GetComponent() const { return TypeComponent<T>(); }
    };
}

#endif
