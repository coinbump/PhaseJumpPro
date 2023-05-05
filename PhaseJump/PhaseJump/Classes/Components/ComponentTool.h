#ifndef PJCOMPONENTTOOL_H
#define PJCOMPONENTTOOL_H

#include "SimplePolygonCollider2D.h"
#include "SpriteRenderer.h"
#include "RenderMaterial.h"

namespace PJ {
    /// Provides smart logic to adding components
    class ComponentTool {
    public:
        template <class T>
        void AddComponent(WorldNode& node, SP<T> component) {
            if (nullptr == component) { return; }
            if (node.AddComponent(component)) {
                AddComponentInternal(node, SCAST<SomeWorldComponent>(component));
            }
        }

    protected:
        void AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component);
    };
}

#endif
