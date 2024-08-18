#pragma once

#include "Camera.h"
#include "InputTriggerMap.h"
#include "List.h"
#include "OrderedMap.h"
#include "PointerClickUIEvent.h"
#include "SomeWorldSystem.h"
#include "UnorderedMap.h"
#include "WorldNode.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;

    struct LocalHit {
        LocalPosition localPos;
        WP<WorldNode> node;

        LocalHit(LocalPosition localPos, WP<WorldNode> node) :
            localPos(localPos),
            node(node) {}
    };

    /// Has logic needed for standard UI behavior (button tracking, pointer exit, etc.)
    class EventSystem : public SomeWorldSystem {
    public:
        using Base = SomeWorldSystem;
        using ControllerId = String;

    public:
        /// Maps UI events to input events
        SP<SomeMultiMap<SomeUIEvent, String>> inputMap = MAKE<InputTriggerMap>();
        std::function<void(EventSystem& system, List<WP<WorldNode>>& nodes)> inputCollectFunc;

    protected:
        /// Collection of nodes that received a pointer down event so they can receive a pointer up
        /// event later, even if the pointer is no longer inside their bounds. This is used for
        /// button tracking
        UnorderedMap<PointerInputButtonType, VectorList<WP<WorldNode>>> pointerDownNodesMap;

        /// List of nodes that received a pointer enter event so they can receive a pointer leave
        /// event later
        List<WP<WorldNode>> pointerEnterNodes;

    public:
        EventSystem();

        void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override;

        std::optional<LocalHit> TestLocalHit(ScreenPosition screenPosition);

        virtual void OnPointerDown(PointerDownUIEvent const& event);
        virtual void OnPointerUp(PointerUpUIEvent const& event);
        virtual void OnMouseMotion(PointerMoveUIEvent const& event);
        virtual void OnKeyDown(KeyDownUIEvent const& event, List<WP<WorldNode>> const& nodes);
        virtual void OnInputAction(InputActionEvent const& event, List<WP<WorldNode>> const& nodes);

    protected:
        template <class Responder>
        void DispatchEvent(SPC<WorldNode> node, std::function<void(Responder&)> command) {
            GUARD(command)

            auto iterComponents = node->Components();
            for (auto& component : iterComponents) {
                auto responder = DCAST<Responder>(component);
                GUARD_CONTINUE(responder)
                command(*responder);
            }
        }
    };
} // namespace PJ
