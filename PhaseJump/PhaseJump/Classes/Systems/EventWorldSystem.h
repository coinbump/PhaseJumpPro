#pragma once

#include "InputMap.h"
#include "List.h"
#include "OrderedMap.h"
#include "PointerClickUIEvent.h"
#include "SomeWorldSystem.h"
#include "UnorderedMap.h"
#include "WorldNode.h"
#include <memory>

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 10/6/24
 */
namespace PJ {
    class WorldNode;
    class DropFilesUIEvent;
    class KeyDownUIEvent;

    /// A node hit with the local position hit inside the node
    struct LocalHit {
        LocalPosition localPos;
        WorldNode& node;

        LocalHit(LocalPosition localPos, WorldNode& node) :
            localPos(localPos),
            node(node) {}
    };

    /// Dispatches UI events to the appropriate node
    /// If you're thinking about using this, you probably want UIWorldSystem instead
    class EventWorldSystem : public WorldSystem {
    public:
        using Base = WorldSystem;
        using This = EventWorldSystem;
        using EventNodeList = VectorList<WorldNode*>;

    public:
        /// Maps UI events to input events
        UP<InputMap> inputMap = NEW<InputMap>();

        /// Collects the list of nodes to dispatch events to
        std::function<void(EventWorldSystem& system, EventNodeList& nodes)> collectEventNodesFunc;

    public:
        EventWorldSystem(String name = "Events");

        /// Tests for nodes hit at this screen position and returns a list of hits
        /// Requires a raycaster and a camera
        virtual VectorList<PJ::LocalHit> TestScreenHit(ScreenPosition screenPosition);

        virtual void OnDropFiles(DropFilesUIEvent const& event, EventNodeList const& nodes);
        virtual void OnPointerDown(PointerDownUIEvent const& event);
        virtual void OnPointerUp(PointerUpUIEvent const& event);
        virtual void OnPointerMove(PointerMoveUIEvent const& event);
        virtual void OnKeyDown(KeyDownUIEvent const& event, EventNodeList const& nodes);
        virtual void
        OnInputAction(SomeSignal const& event, String action, EventNodeList const& nodes);

        // MARK: SomeWorldSystem

        void ProcessUIEvents(UIEventList const& uiEvents) override;

    protected:
        void DispatchEvent(String signalId, SomeSignal const& signal, EventNodeList const& nodes);
        void DispatchEvent(WorldNode& node, String signalId, SomeSignal const& signal);
    };
} // namespace PJ
