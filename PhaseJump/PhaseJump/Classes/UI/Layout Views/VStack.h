#ifndef PJVSTACK_H
#define PJVSTACK_H

#include "HorizontalAlignment.h"
#include "OrderedMap.h"
#include "View2D.h"
#include <optional>

/*
 RATING: 4 stars
 Direct port. Needs unit tests
 CODE REVIEW: 6/18/23
 */
// TODO: Need unit tests
namespace PJ {
    /// Arranges views vertically
    class VStack : public View2D {
    public:
        HorizontalAlignment alignment = HorizontalAlignment::center;
        float spacing = 0;

        /// Derive preferred width of stack from its child views
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

        /// Derive preferred height of stack from its child views
        std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) override {
            auto childViews = ChildViews();
            float totalChildHeight = 0;

            // If all child views have a preferred size, we can derive the
            // preferred height of the VStack
            for (auto& view : childViews) {
                auto childHeight = view->ProposedHeightWithConstraints(layoutSize);
                if (!childHeight) {
                    return std::nullopt;
                }

                totalChildHeight += childHeight.value();
            }

            return std::make_optional(totalChildHeight + (childViews.size() - 1) * spacing);
        }

    protected:
        void _ApplyLayout(Vector2 layoutSize) override {
            auto childViews = ChildViews();

            // Views with no intrinsic size
            List<SP<View2D>> nonIntrinsicChildViews;

            // Views with intrinsic size
            OrderedMap<SP<View2D>, float> intrinsicHeightChildViews;

            auto heightAvailable = layoutSize.y - (childViews.size() - 1) * spacing;
            for (auto& view : childViews) {
                auto intrinsicHeight = view->ProposedHeightWithConstraints(layoutSize);
                if (!intrinsicHeight) {
                    Add(nonIntrinsicChildViews, view);
                } else {
                    intrinsicHeightChildViews[view] = intrinsicHeight.value();
                }
            }

            float totalIntrinsicHeight = 0;
            for (auto& keyAndIntrinsicHeight : intrinsicHeightChildViews) {
                auto intrinsicHeight = keyAndIntrinsicHeight.second;
                totalIntrinsicHeight += intrinsicHeight;
            }

            auto nonIntrinsicViewsCount = nonIntrinsicChildViews.size();
            auto nonIntrinsicTotalHeight = heightAvailable - totalIntrinsicHeight;
            auto nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;
            auto preferredStackWidthOptional = ProposedWidthWithConstraints(layoutSize);
            auto preferredStackWidth =
                preferredStackWidthOptional ? preferredStackWidthOptional.value() : layoutSize.x;

            float y = 0;
            for (auto& view : childViews) {
                auto frame = view->Frame();

                auto preferredWidth =
                    view->ProposedWidthWithConstraints(Vector2(preferredStackWidth, layoutSize.y));
                frame.size.x = preferredWidth ? preferredWidth.value() : preferredStackWidth;

                auto found = intrinsicHeightChildViews.find(view);
                if (found != intrinsicHeightChildViews.end()) {
                    auto intrinsicHeight = (*found).second;
                    frame.size.y = intrinsicHeight;
                } else {
                    auto height = view->ProposedHeightWithConstraints(
                        Vector2(frame.size.x, nonIntrinsicHeight)
                    );
                    frame.size.y = height ? height.value() : nonIntrinsicHeight;
                    nonIntrinsicTotalHeight -= frame.size.y;
                    nonIntrinsicViewsCount--;
                    nonIntrinsicHeight = nonIntrinsicTotalHeight / nonIntrinsicViewsCount;
                }

                frame.origin.y = y;
                frame.origin.x = alignment.aligner->AlignedOrigin(layoutSize.x, frame.size.x);

                view->SetFrame(frame);

                y += frame.size.y;
                y += spacing;
            }
        }
    };
} // namespace PJ

#endif
