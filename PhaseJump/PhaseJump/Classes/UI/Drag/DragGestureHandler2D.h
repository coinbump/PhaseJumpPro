#pragma once

#include "DragHandler.h"

/*
 RATING: 5+ stars
 Tested and works
 CODE REVIEW: 4/15/26
 */
namespace PJ {
    /// Updates with drag state while gesture is in progress
    class DragGestureHandler2D : public DragHandler {
    public:
        struct Update {
            enum class Type {
                /// Drag started
                Start,

                /// Drag is in progress
                Update,

                /// Drag ended
                End,

                /// Drag cancelled
                Cancel
            };

            DragGestureHandler2D& handler;
            Type type;

            /// Start world position of the drag gesture
            WorldPosition2D startPosition;

            /// Current world position of the drag gesture
            WorldPosition2D position;

            /// @return Returns total translation from the start of the drag gesture to the current
            /// event of the drag gesture
            Vector2 Translation() const {
                return position - startPosition;
            }
        };

        using Base = DragHandler;
        using OnDragGestureUpdateFunc = std::function<void(Update)>;

        OnDragGestureUpdateFunc onDragGestureUpdateFunc;

        DragGestureHandler2D() {
            onDragUpdateFunc = [this](auto& dragHandler, auto position) {
                OnDragGestureUpdate({ .handler = *this,
                                      .type = Update::Type::Update,
                                      .startPosition = dragStartPosition,
                                      .position = position });
            };
        }

    protected:
        virtual void OnDragGestureUpdate(Update update) {
            GUARD(onDragGestureUpdateFunc)
            onDragGestureUpdateFunc(update);
        }

        // MARK: DragHandler

        void OnDragCancel() override {
            Base::OnDragCancel();

            OnDragGestureUpdate({ .handler = *this, .type = Update::Type::Cancel });
        }

        void OnDragStart(WorldPosition position) override {
            Base::OnDragStart(position);

            OnDragGestureUpdate({ .handler = *this,
                                  .type = Update::Type::Start,
                                  .startPosition = dragStartPosition,
                                  .position = position });
        }

        void OnDragEnd(WorldPosition position) override {
            Base::OnDragEnd(position);

            OnDragGestureUpdate({ .handler = *this,
                                  .type = Update::Type::End,
                                  .startPosition = dragStartPosition,
                                  .position = position });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DragGestureHandler2D";
        }
    };
} // namespace PJ
