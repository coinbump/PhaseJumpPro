#pragma once

#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class World;

    /// Draws standard world info
    class EditorImGuiWorldInfoPainter : public SomeImGuiPainter {
    public:
        World& world;

        EditorImGuiWorldInfoPainter(World& world);
    };
} // namespace PJ
