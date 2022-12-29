#ifndef PJSOMENODE_H
#define PJSOMENODE_H

#include "WorldComponent.h"
#include "_String.h"
#include "_Set.h"
#include "Vector3.h"
#include <memory>

namespace PJ {
    enum class NodeCullType
    {
        Invisible, ZeroAlpha
    };

    class SomeNode : public WorldComponent {
    public:
        String id;

        std::shared_ptr<Set<String>> typeTags;
        std::shared_ptr<Set<String>> stateTags;
        std::shared_ptr<Set<NodeCullType>> cullTypes;

        bool HasTypeTag(String name) const {
            return typeTags->Contains(name);
        }
        
        bool HasStateTag(String name) const {
            return stateTags->Contains(name);
        }

        virtual bool IsKinematic() const = 0;
        virtual void MoveToPosition(Vector3 position, bool force = false);
    };
}

#endif
