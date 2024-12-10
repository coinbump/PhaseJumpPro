#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/17/24
 */
namespace PJ {
    /// Insets from layout edges
    struct LayoutInsets {
        using This = LayoutInsets;

        float top{};
        float bottom{};
        float leading{};
        float trailing{};

        static This Uniform(float value) {
            return { .leading = value, .trailing = value, .top = value, .bottom = value };
        }

        /// @return Returns insets with uniform horizontal values
        static LayoutInsets Horizontal(float value) {
            return { .leading = value, .trailing = value };
        }

        /// @return Returns insets with uniform vertical values
        static LayoutInsets Vertical(float value) {
            return { .top = value, .bottom = value };
        }

        float HorizontalSum() const {
            return leading + trailing;
        }

        float VerticalSum() const {
            return top + bottom;
        }

        Vector2 Sum() const {
            return { HorizontalSum(), VerticalSum() };
        }

        This operator+(This const& rhs) const {
            This result = *this;
            result += rhs;
            return result;
        }

        This& operator+=(This const& rhs) {
            auto& result = *this;

            result.top += rhs.top;
            result.bottom += rhs.bottom;
            result.leading += rhs.leading;
            result.trailing += rhs.trailing;

            return result;
        }

        bool operator==(const This& rhs) const {
            return top == rhs.top && bottom == rhs.bottom && leading == rhs.leading &&
                   trailing == rhs.trailing;
        }
    };
} // namespace PJ
