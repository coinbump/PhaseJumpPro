#pragma once

#include "DragModel.h"
#include "EventWorldSystem.h"
#include "OrderedSet.h"
#include "SDLMouseDevice.h"
#include "SelectHandler.h"
#include "SomeCamera.h"
#include "SomePointerUIEvent.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 1/9/25
 */
namespace PJ {
    class SomeUIEvent;
    class SomeMouseDevice;
    class SomeFocusCoordinator;
    class FocusHandler;

    /// Sent when a drag enters a drop area
    class DragEnterUIEvent : public SomeSignal {
    public:
        DragModel dragModel;

        DragEnterUIEvent(DragModel dragModel) :
            dragModel(dragModel) {}
    };

    /// Sent when a drag exits a drop area
    class DragExitUIEvent : public SomeSignal {};

    /// Handles advanced user input like drag and drop, hover, etc.
    class UIWorldSystem : public EventWorldSystem {
    public:
        using Base = EventWorldSystem;
        using This = UIWorldSystem;

    protected:
        /// Active hover node. Only 1 node can have hover status at a time
        WP<WorldNode> activeHover;

        /// Active drop target during a drag. Only 1 drop target can be active at a time
        WP<WorldNode> activeDropTarget;

        /// Active focus. Focus takes keyboard input
        WP<WorldNode> focus;

    public:
        /// @return Returns the camera that defines the projection for this system
        virtual SomeCamera* Camera() const;

        /// @return Returns the active hover node
        SP<WorldNode> ActiveHover();

        /// Sets the active hover node
        void SetActiveHover(SP<WorldNode> component);

        /// Updates the active hover node based on collision hits
        void UpdateActiveHover(VectorList<PJ::LocalHit> hits);

        // MARK: - Selection

    protected:
        /// Collection of select handler components that define the current object selection
        UnorderedSet<SP<SelectHandler>> selection;

        /// Collection of nodes that received a pointer down event so they can receive a pointer up
        /// event later, even if the pointer is no longer inside their bounds. This is used for
        /// button tracking
        UnorderedMap<PointerInputButtonType, UnorderedSet<SP<WorldNode>>> pointerDownNodesMap;

        /// Nodes that received a pointer enter event so they can receive a pointer leave event
        /// later
        UnorderedSet<SP<WorldNode>> pointerEnterNodes;

    public:
        UIWorldSystem(String name = "UI") :
            Base(name) {}

        virtual ~UIWorldSystem() {}

        /// @return Returns the current selection
        UnorderedSet<SP<SelectHandler>> const& Selection() const {
            return selection;
        }

        /// Sets the selection
        void SetSelection(UnorderedSet<SP<SelectHandler>> const& value);

        /// Updates the selection for a handler
        void UpdateSelectionFor(SP<SelectHandler> selectHandler);

        /// Called to start a drag objects gesture
        virtual void StartDrag(SP<DragModel> dragModel);

        /// @return Returns the drag objects gesture model
        DragModel* DraggedItems() const {
            return dragModel.get();
        }

        /// @return Returns true if a drag objects gesture is active
        bool IsDragging() const {
            return dragState != DragState::Default;
        }

        // MARK: EventWorldSystem

        void OnPointerMove(PointerMoveUIEvent const& event) override;
        void OnPointerUp(PointerUpUIEvent const& pointerUpEvent) override;
        void OnPointerDown(PointerDownUIEvent const& pointerDownEvent) override;

        SP<SomeMouseDevice> mouseDevice = MAKE<SDLMouseDevice>();

        // MARK: SomeWorldComponent

        FinishType OnUpdate(TimeSlice time) override;

    protected:
        // MARK: SomeWorldComponent

        enum class DragState { Default, Drag, LockDragMouseDown, LockDragMouseUp };

        /// If true, a click will start the drag, and a click will end it
        bool lockDrag{};

        /// Model that defines a drag in progress
        SP<DragModel> dragModel;

        /// State of drag in progress
        DragState dragState = DragState::Default;

        /// Called to finalize drag objects gesture
        virtual void OnDragEnd();

        /// Called up to date active drop targets
        virtual void CheckDropTargets(ScreenPosition screenPos);

        /// Called during drag objects gesture
        virtual void OnDragUpdate();
    };
} // namespace PJ
