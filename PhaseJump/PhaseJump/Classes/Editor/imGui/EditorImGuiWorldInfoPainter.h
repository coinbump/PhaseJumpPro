#pragma once

#include "ImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class World;

    /// Draws standard world info
    class EditorImGuiWorldInfoPainter : public ImGuiPainter {
    public:
        World& world;

        EditorImGuiWorldInfoPainter(World& world);
    };
} // namespace PJ
