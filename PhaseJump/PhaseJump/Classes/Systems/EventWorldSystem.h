#pragma once

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
    class DropFilesUIEvent;

    struct LocalHit {
        LocalPosition localPos;
        WP<WorldNode> node;

        LocalHit(LocalPosition localPos, WP<WorldNode> node) :
            localPos(localPos),
            node(node) {}
    };

    /// Has logic needed for standard UI behavior (button tracking, pointer exit, etc.)
    class EventWorldSystem : public SomeWorldSystem {
    public:
        using Base = SomeWorldSystem;
        using ControllerId = String;

    public:
        /// Maps UI events to input events
        SP<SomeMultiMap<SomeUIEvent, String>> inputMap = MAKE<InputTriggerMap>();
        std::function<void(EventWorldSystem& system, List<WP<WorldNode>>& nodes)> inputCollectFunc;

    protected:
        /// Collection of nodes that received a pointer down event so they can receive a pointer up
        /// event later, even if the pointer is no longer inside their bounds. This is used for
        /// button tracking
        UnorderedMap<PointerInputButtonType, VectorList<WP<WorldNode>>> pointerDownNodesMap;

        /// List of nodes that received a pointer enter event so they can receive a pointer leave
        /// event later
        List<WP<WorldNode>> pointerEnterNodes;

    public:
        EventWorldSystem(String name = "Events");

        void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override;

        std::optional<LocalHit> TestLocalHit(ScreenPosition screenPosition);

        virtual void OnDropFiles(DropFilesUIEvent const& event, List<WP<WorldNode>> const& nodes);
        virtual void OnPointerDown(PointerDownUIEvent const& event);
        virtual void OnPointerUp(PointerUpUIEvent const& event);
        virtual void OnMouseMotion(PointerMoveUIEvent const& event);
        virtual void OnKeyDown(KeyDownUIEvent const& event, List<WP<WorldNode>> const& nodes);
        virtual void OnInputAction(InputActionEvent const& event, List<WP<WorldNode>> const& nodes);

    protected:
        void DispatchEvent(SP<WorldNode> node, String signalId, SomeSignal const& signal) {
            GUARD(node && node->IsEnabled());

            auto iterComponents = node->Components();
            std::for_each(iterComponents.begin(), iterComponents.end(), [&](auto& component) {
                GUARD(component->IsEnabled())
                try {
                    auto i = component->signalHandlers.at(signalId);
                    i(*component, signal);
                } catch (...) {}
            });
        }
    };
} // namespace PJ
