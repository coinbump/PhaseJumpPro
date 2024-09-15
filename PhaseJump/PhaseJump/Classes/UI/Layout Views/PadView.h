#ifndef PJPADVIEW_H
#define PJPADVIEW_H

#include "View2D.h"

/*
 RATING: 4 stars
 Direct port. Needs unit tests
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// View that adds intrinsic padding to child
    class PadView : public View2D {
    public:
        using Base = View2D;

    protected:
        float padLeading = 0;
        float padTrailing = 0;
        float padTop = 0;
        float padBottom = 0;

    public:
        float PadLeading() const {
            return padLeading;
        }

        float PadTrailing() const {
            return padTrailing;
        }

        float PadTop() const {
            return padTop;
        }

        float PadBottom() const {
            return padBottom;
        }

        std::optional<float> ProposedWidthWithoutConstraints(Vector2 layoutSize) override {
            auto firstChild = FirstChildView();
            if (!firstChild) {
                return Base::ProposedWidthWithoutConstraints(layoutSize);
            }

            auto preferredWidth = firstChild->ProposedWidthWithConstraints(layoutSize);
            auto result = preferredWidth ? preferredWidth.value() : 0;

            result += PadLeading();
            result += PadTrailing();

            return std::make_optional(result);
        }

        std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) override {
            auto firstChild = FirstChildView();
            if (!firstChild) {
                return Base::ProposedHeightWithoutConstraints(layoutSize);
            }

            auto preferredHeight = firstChild->ProposedHeightWithConstraints(layoutSize);
            auto result = preferredHeight ? preferredHeight.value() : 0;

            result += PadTop();
            result += PadBottom();

            return std::make_optional(result);
        }

    protected:
        void _ApplyLayout(Vector2 layoutSize) override {
            auto firstChild = FirstChildView();
            if (!firstChild) {
                return;
            }

            Rect frame;

            float layoutSizeX = layoutSize.x - PadLeading() - PadTrailing();
            auto childPreferredWidth =
                firstChild->ProposedWidthWithConstraints(Vector2(layoutSizeX, layoutSize.y));
            frame.size.x = childPreferredWidth ? childPreferredWidth.value() : 0;

            auto childPreferredHeight = firstChild->ProposedHeightWithConstraints(
                Vector2(frame.size.x, layoutSize.y - PadTop() - PadBottom())
            );

            frame.size.y = childPreferredHeight ? childPreferredHeight.value() : 0;
            frame.origin.x = PadLeading();
            frame.origin.y = PadTop();

            firstChild->SetFrame(frame);
        } // TESTED
    };
} // namespace PJ

#endif
