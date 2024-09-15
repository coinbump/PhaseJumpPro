#ifndef PJSOMEDROPTARGET_H
#define PJSOMEDROPTARGET_H

#include "DragItems.h"
#include "SomeCollider2D.h"
#include "SomeEffect.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// Target that can accept drag items
    /// Requres that the object have a collider attached for raycast checks
    class SomeDropTarget : public WorldComponent<> {
    public:
        using Base = WorldComponent;

        SP<SomeEffect> acceptDragEffect;

        enum class StateType { NotOver, DragOverAccept, DragOverReject };

    protected:
        StateType state = StateType::NotOver;

    public:
        StateType State() const {
            return state;
        }

        void SetState(StateType value) {
            if (state == value) {
                return;
            }
            state = value;
            OnStateChange();
        }

        virtual bool CanAcceptDrag(DragItems const& items) {
            // Debug.Log("CanAcceptDrag");
            return true;
        }

        virtual void OnDragEnter(DragItems const& items) {
            // Debug.Log("OnDragEnter");
            SetState(CanAcceptDrag(items) ? StateType::DragOverAccept : StateType::DragOverReject);
        }

        virtual void OnDragLeave() {
            // Debug.Log("OnDragLeave");
            SetState(StateType::NotOver);
        }

        virtual void OnAcceptDrag(DragItems const& items) {}

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SomeDropTarget";
        }

    protected:
        void Awake() override {
            Base::Awake();

#if DEBUG
//            if (NULL == GetComponent<SomeCollider2D>()) {
//                PJ::Log("Error. DropTarget requires a collider.");
//            }
#endif
        }

        void UpdateAcceptDragEffect() {
            if (acceptDragEffect) {
                acceptDragEffect->SetIsOn(State() == StateType::DragOverAccept);
            }
        }

        virtual void OnStateChange() {
            UpdateAcceptDragEffect();
        }
    };
} // namespace PJ

#endif
