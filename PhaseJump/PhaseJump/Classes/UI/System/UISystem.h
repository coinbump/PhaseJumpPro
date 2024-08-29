#pragma once

#include "Camera.h"
#include "DragItems.h"
#include "DragModel.h"
#include "EventSystem.h"
#include "OrderedSet.h"
#include "SDLMouseDevice.h"
#include "SelectHandler.h"
#include "SomePointerUIEvent.h"
#include <memory>

/*
 Needs unit tests
 */
// CODE REVIEW: 5/9/23
namespace PJ {
    class SomeUIEvent;
    class SomeMouseDevice;

    /// Handled advanced user input like drag and drop, hover, etc.
    class UISystem : public EventSystem {
    public:
        using Base = EventSystem;

        // TODO: fix this, it's causing problems when switching scenes
        static UISystem* shared;

        virtual SP<SomeCamera> Camera() const;

        void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override;

        // *****************
        // MARK: - Selection

    protected:
        OrderedSet<SP<SelectHandler>> selection;

    public:
        virtual ~UISystem() {
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
        virtual void CheckDropTargets();
        virtual void OnDragUpdate();

        DragItems DraggedItems() const {
            if (nullptr != dragModel) {
                VectorList<SP<SomeDragHandler>> list{ dragModel->dragHandler };
                return DragItems(list);
            } else {
                return DragItems();
            }
        }

        bool IsDragging() const {
            return dragState != DragState::Default;
        }

    protected:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;

        void OnPointerDown(PointerDownUIEvent const& pointerDownEvent) override;
        void OnPointerUp(PointerUpUIEvent const& pointerUpEvent) override;

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
