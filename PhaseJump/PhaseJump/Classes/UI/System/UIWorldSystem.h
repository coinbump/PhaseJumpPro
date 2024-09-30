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
 Needs unit tests
 */
// CODE REVIEW: 5/9/23
namespace PJ {
    class SomeUIEvent;
    class SomeMouseDevice;
    class SomeFocusCoordinator;
    class FocusHandler;

    class DragEnterUIEvent : public SomeUIEvent {
    public:
        DragModel dragModel;

        DragEnterUIEvent(DragModel dragModel) :
            dragModel(dragModel) {}
    };

    class DragExitUIEvent : public SomeUIEvent {};

    /// Handled advanced user input like drag and drop, hover, etc.
    class UIWorldSystem : public EventWorldSystem {
    public:
        using Base = EventWorldSystem;

        // TODO: fix this, it's causing problems when switching scenes
        static UIWorldSystem* shared;

        ScreenPosition mousePosition;
        bool hasMousePosition{};

        WP<WorldNode> activeHover;
        WP<WorldNode> activeDropTarget;

        virtual SP<SomeCamera> Camera() const;

        void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override;

        SP<WorldNode> ActiveHover();
        void SetActiveHover(SP<WorldNode> component);
        void UpdateActiveHover(VectorList<PJ::LocalHit> hits);

        // *****************
        // MARK: - Selection

    protected:
        OrderedSet<SP<SelectHandler>> selection;

        /// Collection of nodes that received a pointer down event so they can receive a pointer up
        /// event later, even if the pointer is no longer inside their bounds. This is used for
        /// button tracking
        UnorderedMap<PointerInputButtonType, VectorList<WP<WorldNode>>> pointerDownNodesMap;

        /// Nodes that received a pointer enter event so they can receive a pointer leave event
        /// later
        List<WP<WorldNode>> pointerEnterNodes;

    public:
        // TODO: VectorList<UP<SomeFocusCoordinator>> focusCoordinators;
        WP<FocusHandler> focus;

        UIWorldSystem(String name = "UI") :
            Base(name) {}

        virtual ~UIWorldSystem() {
            GUARD(shared == this);
            shared = nullptr;
        }

        OrderedSet<SP<SelectHandler>> const& Selection() const {
            return selection;
        }

        void SetSelection(OrderedSet<SP<SelectHandler>> const& value);
        void UpdateSelectionFor(SP<SelectHandler> selectHandler);

        virtual void StartDrag(SP<DragModel> dragModel);
        virtual void OnDragEnd();
        virtual void CheckDropTargets(ScreenPosition screenPos);
        virtual void OnDragUpdate();

        DragModel* DraggedItems() const {
            return dragModel.get();
        }

        bool IsDragging() const {
            return dragState != DragState::Default;
        }

        // MARK: EventWorldSystem

        void OnMouseMotion(PointerMoveUIEvent const& event) override;
        void OnPointerUp(PointerUpUIEvent const& pointerUpEvent) override;
        void OnPointerDown(PointerDownUIEvent const& pointerDownEvent) override;

    protected:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;

        // ****************************************
        // MARK: - UISystem+Drag
        // ****************************************

        enum class DragState { Default, Drag, LockDragMouseDown, LockDragMouseUp };

        /// If true, a click will start the drag, and a click will end it
        bool lockDrag = false;

        /// Model that defines a drag in progress
        SP<DragModel> dragModel;

        /// Object that a drag is over
        WP<WorldNode> dropTargetOverObject;

        /// State of drag in progress
        DragState dragState = DragState::Default;
        SP<SomeMouseDevice> mouseDevice = MAKE<SDLMouseDevice>();
    };
} // namespace PJ
