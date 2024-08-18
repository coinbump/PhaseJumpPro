#ifndef PJZSTACK_H
#define PJZSTACK_H

#include "Alignment.h"
#include "View2D.h"
#include <optional>

/*
 RATING: 4 stars
 Direct port. Needs unit tests
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// Arranges views in z-space stacked on top of each other
    class ZStack : View2D {
    public:
        Alignment alignment = Alignment::center;
        float spacing = 0;

        std::optional<float> ProposedWidthWithoutConstraints(Vector2 layoutSize) override {
            float result = 0;
            auto childViews = ChildViews();
            bool isIntrinsic = false;

            for (auto& view : childViews) {
                auto childWidth = view->ProposedWidthWithConstraints(layoutSize);
                if (childWidth) {
                    isIntrinsic = true;
                    result = std::max(result, childWidth.value());
                }
            }

            return isIntrinsic ? std::make_optional(result) : std::nullopt;
        }

        std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) override {
            float result = 0;
            auto childViews = ChildViews();
            auto isIntrinsic = false;

            for (auto& view : childViews) {
                auto childHeight = view->ProposedHeightWithConstraints(layoutSize);
                if (childHeight) {
                    isIntrinsic = true;
                    result = std::max(result, childHeight.value());
                }
            }

            return isIntrinsic ? std::make_optional(result) : std::nullopt;
        }

    protected:
        void _ApplyLayout(Vector2 layoutSize) override {
            auto childViews = ChildViews();

            auto preferredStackWidthOptional = ProposedWidthWithoutConstraints(layoutSize);
            auto preferredStackWidth =
                preferredStackWidthOptional ? preferredStackWidthOptional.value() : layoutSize.x;

            auto preferredStackHeightOptional = ProposedHeightWithoutConstraints(layoutSize);
            auto preferredStackHeight =
                preferredStackHeightOptional ? preferredStackHeightOptional.value() : layoutSize.y;

            for (auto& view : childViews) {
                Rect frame;

                auto childPreferredWidth = view->ProposedWidthWithConstraints(layoutSize);
                auto childPreferredHeight = view->ProposedHeightWithConstraints(layoutSize);

                auto childWidth =
                    childPreferredWidth ? childPreferredWidth.value() : preferredStackWidth;
                auto childHeight =
                    childPreferredHeight ? childPreferredHeight.value() : preferredStackHeight;

                frame.size = Vector2(childWidth, childHeight);
                frame.origin.x = alignment.horizontalAlignment.aligner->AlignedOrigin(
                    layoutSize.x, frame.size.x
                );
                frame.origin.y =
                    alignment.verticalAlignment.aligner->AlignedOrigin(layoutSize.y, frame.size.y);

                view->SetFrame(frame);
            }
        }
    };
} // namespace PJ

#endif
