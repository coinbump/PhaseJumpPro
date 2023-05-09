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
    /// Adds convenience methods that operate on WorldNode
    /// We can't declare these in SomeWorldComponent because it would create a circular include between SomeWorldComponent and WorldNode
    /// </summary>
    class WorldComponent : public SomeWorldComponent {
    public:
        template <class T>
        SP<T> TypeComponent() const
        {
            if (owner.expired()) { return nullptr; }
            return owner.lock()->TypeComponent<T>();
        }

        template <class T>
        VectorList<SP<T>> TypeComponents() const
        {
            if (owner.expired()) { return nullptr; }
            return owner.lock()->TypeComponents<T>();
        }

        template <class T>
        VectorList<SP<T>> TypeComponentsInChildren() {
            VectorList<SP<T>> result;

            if (owner.expired()) { return result; }
            return owner.lock()->TypeComponentsInChildren<T>();
        }

        VectorList<SP<WorldNode>> ChildNodes() const
        {
            if (owner.expired()) { return VectorList<SP<WorldNode>>(); }
            return owner.lock()->ChildNodes();
        }

        // Convenience names
        template <class T>
        SP<T> GetComponent() const { return TypeComponent<T>(); }
        template <class T>
        VectorList<SP<T>> GetComponentsInChildren() const { return TypeComponentsInChildren<T>(); }
        template <class T>
        VectorList<SP<T>> GetComponents() const { return TypeComponents<T>(); }
    };
}

#endif
