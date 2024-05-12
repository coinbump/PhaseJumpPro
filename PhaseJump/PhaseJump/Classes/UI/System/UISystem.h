#ifndef PJUISYSTEM_H
#define PJUISYSTEM_H

#include "EventSystem.h"
#include "DragModel.h"
#include "SDLMouseDevice.h"
#include "SomePointerUIEvent.h"
#include "DragItems.h"
#include "Camera.h"
#include "SelectHandler.h"
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
        
        static SP<UISystem> shared;

        virtual SP<SomeCamera> Camera() const { return Camera::main; }

        void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) override;

        // *****************
        // MARK: - Selection

    protected:
        Set<SP<SelectHandler>> selection;

    public:
        Set<SP<SelectHandler>> const& Selection() const { return selection; }
        void SetSelection(Set<SP<SelectHandler>> const& value);
        void UpdateSelectionFor(SP<SelectHandler> selectHandler);

    public:
        virtual void StartDrag(SP<DragModel> dragModel);
        virtual void OnDragEnd();
        virtual void CheckDropTargets();
        virtual void OnDragUpdate();

        DragItems DraggedItems() const
        {
            if (nullptr != dragModel)
            {
                VectorList<SP<SomeDragHandler>> list { dragModel->dragHandler };
                return DragItems(list);
            }
            else
            {
                return DragItems();
            }
        }

        bool IsDragging() const
        {
            return dragState != DragState::Default;
        }

    protected:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;

        bool ProcessPointerDownEvent(SP<PointerDownUIEvent<ScreenPosition>> pointerDownEvent) override;
        bool ProcessPointerUpEvent(SP<PointerUpUIEvent> pointerUpEvent) override;

        // ****************************************
        // MARK: - UISystem+Drag
        // ****************************************

        enum class DragState
        {
            Default, Drag, LockDragMouseDown, LockDragMouseUp
        };

        /// <summary>
        /// If true, a click will start the drag, and a click will end it
        /// </summary>
        bool lockDrag = false;

        /// Model that defines a drag in progress
        SP<DragModel> dragModel;

        /// Object that a drag is over
        WP<WorldNode> dropTargetOverObject;

        /// State of drag in progress
        DragState dragState = DragState::Default;
        SP<SomeMouseDevice> mouseDevice = MAKE<SDLMouseDevice>();
    };
}

#endif
