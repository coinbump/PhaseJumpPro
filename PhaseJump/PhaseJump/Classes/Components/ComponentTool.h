#ifndef PJCOMPONENTTOOL_H
#define PJCOMPONENTTOOL_H

#include "Colliders2D.h"
#include "RenderMaterial.h"
#include "SpriteRenderer.h"
#include "WorldNode.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: do we need to put these inside a struct?
    /// Provides smart logic to adding components
    class ComponentTool {
    public:
        template <class T>
        void AddComponent(WorldNode& node, SP<T> component) {
            if (nullptr == component) {
                return;
            }
            node.Add(component);
            AddComponentInternal(node, SCAST<SomeWorldComponent>(component));
        }

    protected:
        void AddComponentInternal(WorldNode& node, SP<SomeWorldComponent> component);
    };
} // namespace PJ

#endif
