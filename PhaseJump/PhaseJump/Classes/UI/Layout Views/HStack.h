#ifndef PJHSTACK_H
#define PJHSTACK_H

#include "OrderedMap.h"
#include "VerticalAlignment.h"
#include "View2D.h"
#include <optional>

/*
 RATING: 4 stars
 Direct port. Needs Unit Testes
 CODE REVIEW: 6/18/23
 */
// TODO: Need unit tests
namespace PJ {
    /// Arranges views horizontally
    class HStack : public View2D {
    public:
        using View2D = Base;

        VerticalAlignment alignment = VerticalAlignment::center;
        float spacing = 0;

        /// Derive preferred width of stack from its child views
        std::optional<float> ProposedWidthWithoutConstraints(Vector2 layoutSize) override {
            auto childViews = ChildViews();
            float totalChildWidth = 0;

            // If all child views have a preferred size, we can derive the
            // preferred width of the HStack
            for (auto& view : childViews) {
                auto childWidth = view->ProposedWidthWithConstraints(layoutSize);
                if (NULL == childWidth) {
                    return std::nullopt;
                }

                totalChildWidth += childWidth.value();
            }

            return std::make_optional(totalChildWidth + (childViews.size() - 1) * spacing);
        }

        /// Derive preferred height of stack from its child views
        std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) override {
            float result = 0;
            auto childViews = ChildViews();
            bool isIntrinsic = false;

            for (auto& view : childViews) {
                auto childHeight = view->ProposedHeightWithConstraints(layoutSize);
                if (NULL != childHeight) {
                    isIntrinsic = true;
                    result = std::max(result, childHeight.value());
                }
            }

            return isIntrinsic ? std::make_optional(result) : std::nullopt;
        }

    protected:
        void _ApplyLayout(Vector2 layoutSize) override {
            auto childViews = ChildViews();

            // Views with no intrinsic size
            List<SP<View2D>> nonIntrinsicChildViews;

            // Views with intrinsic size
            OrderedMap<SP<View2D>, float> intrinsicWidthChildViews;

            float widthAvailable = layoutSize.x - (childViews.size() - 1) * spacing;
            auto preferredStackHeightOptional = ProposedHeightWithConstraints(layoutSize);
            float preferredStackHeight = preferredStackHeightOptional != NULL
                                             ? preferredStackHeightOptional.value()
                                             : layoutSize.y;

            for (auto& view : childViews) {
                auto intrinsicWidth = view->ProposedWidthWithConstraints(layoutSize);
                if (!intrinsicWidth) {
                    Add(nonIntrinsicChildViews, view);
                } else {
                    intrinsicWidthChildViews[view] = intrinsicWidth.value();
                }
            }

            float totalIntrinsicWidth = 0;
            for (auto& keyAndIntrinsicWidth : intrinsicWidthChildViews) {
                auto intrinsicWidth = keyAndIntrinsicWidth.second;
                totalIntrinsicWidth += intrinsicWidth;
            }

            auto nonIntrinsicViewsCount = nonIntrinsicChildViews.size();
            float nonIntrinsicTotalWidth = widthAvailable - totalIntrinsicWidth;
            float nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;

            float x = 0;
            for (auto& view : childViews) {
                auto frame = view->Frame();

                auto found = intrinsicWidthChildViews.find(view);
                if (found != intrinsicWidthChildViews.end()) {
                    auto intrinsicWidth = (*found).second;
                    frame.size.x = intrinsicWidth;
                } else {
                    auto preferredWidth =
                        view->ProposedWidthWithConstraints(Vector2(nonIntrinsicWidth, layoutSize.y)
                        );
                    auto width = preferredWidth ? preferredWidth.value() : nonIntrinsicWidth;
                    frame.size.x = width;
                    nonIntrinsicTotalWidth -= width;
                    nonIntrinsicViewsCount--;
                    nonIntrinsicWidth = nonIntrinsicTotalWidth / nonIntrinsicViewsCount;
                }

                auto preferredHeight =
                    view->ProposedHeightWithConstraints(Vector2(frame.size.x, preferredStackHeight)
                    );
                frame.size.y = preferredHeight ? preferredHeight.value() : preferredStackHeight;

                frame.origin.x = x;
                frame.origin.y = alignment.aligner(preferredStackHeight, frame.size.y);
                view->SetFrame(frame);

                x += frame.size.x;
                x += spacing;
            }
        }
    };
} // namespace PJ

#endif
