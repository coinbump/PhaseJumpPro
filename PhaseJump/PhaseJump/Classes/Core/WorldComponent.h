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
        std::shared_ptr<T> GetComponent() const
        {
            if (owner.expired()) { return nullptr; }
            return owner.lock()->GetComponent<T>();
        }

        void DestroyOwner(float afterSeconds = 0);

        std::shared_ptr<List<std::shared_ptr<WorldNode>>> ChildNodes() const
        {
            if (owner.expired()) { return nullptr; }
            return owner.lock()->ChildNodes();
        }

        // TODO: pull in code from WorldComponentBeta
    };
}

#endif
