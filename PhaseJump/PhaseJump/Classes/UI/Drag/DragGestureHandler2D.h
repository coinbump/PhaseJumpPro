#pragma once

#include "DragHandler.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/4/24
 */
namespace PJ {
    /// Updates with deltas of the drag while it is in progress
    class DragGestureHandler2D : public DragHandler {
    public:
        struct Update {
            enum class Type {
                // Drag started
                Start,

                // Drag is in progress
                Update,

                // Drag has ended
                End
            };

            DragGestureHandler2D& handler;
            Type type;
            Vector2 worldPosition;
            Vector2 delta;

            Update(DragGestureHandler2D& gesture, Type type, Vector2 worldPosition, Vector2 delta) :
                handler(gesture),
                type(type),
                worldPosition(worldPosition),
                delta(delta) {}
        };

        using Base = DragHandler;
        using OnDragGestureUpdateFunc = std::function<void(Update)>;

        OnDragGestureUpdateFunc onDragGestureUpdateFunc;

        virtual void OnDragGestureUpdate(Update update) {
            GUARD(onDragGestureUpdateFunc)
            onDragGestureUpdateFunc(update);
        }

        // MARK: DragHandler

        void OnDragStart(WorldPosition inputPosition) override {
            OnDragGestureUpdate({ *this, Update::Type::Start, inputPosition, {} });
        }

        void OnDragUpdate(WorldPosition inputPosition) override {
            auto delta = inputPosition - dragStartInputPosition;
            OnDragGestureUpdate({ *this, Update::Type::Update, inputPosition,
                                  Vector2(delta.x, delta.y) });
        }

        void OnDragEnd() override {
            Base::OnDragEnd();

            OnDragGestureUpdate({ *this, Update::Type::End, {}, {} });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DragGestureHandler2D";
        }
    };
} // namespace PJ
