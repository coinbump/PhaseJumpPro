#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    /// Normalized point inside a view (0-1.0)
    /// Example: Center is {0.5, 0.5}
    class ViewPoint {
    public:
        float x{};
        float y{};

        static const ViewPoint center;
        static const ViewPoint leading;
        static const ViewPoint trailing;
        static const ViewPoint topLeading;
        static const ViewPoint topTrailing;
        static const ViewPoint bottomLeading;
        static const ViewPoint bottomTrailing;
    };
} // namespace PJ
