#ifndef PJSOMENODE_H
#define PJSOMENODE_H

#include "OrderedSet.h"
#include "StringUtils.h"
#include "Vector3.h"
#include "WorldComponent.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: Use struct-composition or string for these, not enum
    enum class NodeCullType { Invisible, ZeroAlpha };

    // TODO: rethink naming (everything is a "node")
    class SomeNode : public WorldComponent<> {
    public:
        String id;

        SP<OrderedSet<String>> typeTags;
        SP<OrderedSet<String>> stateTags;
        SP<OrderedSet<NodeCullType>> cullTypes;

        bool HasTypeTag(String name) const {
            GUARDR(typeTags, false)
            return Contains(*typeTags, name);
        }

        bool HasStateTag(String name) const {
            GUARDR(stateTags, false)
            return Contains(*stateTags, name);
        }

        virtual bool IsKinematic() const = 0;
        virtual void MoveToPosition(Vector3 position, bool force = false) = 0;
    };
} // namespace PJ

#endif
