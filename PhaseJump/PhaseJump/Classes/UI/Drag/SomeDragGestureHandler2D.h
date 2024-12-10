#pragma once

#include "SomeDragHandler.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/4/24
 */
namespace PJ {
    // TODO: remove some? (because of funcs)
    /// Updates with deltas of the drag while it is in progress
    class SomeDragGestureHandler2D : public SomeDragHandler {
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

            SomeDragGestureHandler2D& gesture;
            Type type;
            Vector2 worldPosition;
            Vector2 delta;

            Update(
                SomeDragGestureHandler2D& gesture, Type type, Vector2 worldPosition, Vector2 delta
            ) :
                gesture(gesture),
                type(type),
                worldPosition(worldPosition),
                delta(delta) {}
        };

        using Base = SomeDragHandler;
        using OnDragGestureUpdateFunc = std::function<void(Update)>;

        OnDragGestureUpdateFunc onDragGestureUpdateFunc;

        virtual void OnDragGestureUpdate(Update update) {
            GUARD(onDragGestureUpdateFunc)
            onDragGestureUpdateFunc(update);
        }

        // MARK: SomeDragHandler

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
            return "SomeDragGestureHandler2D";
        }
    };
} // namespace PJ
