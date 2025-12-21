#pragma once

#include "ImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    class EditorWorldSystem;

    /// Draws list of world resources, with info and tools
    class EditorImGuiPrefabsPainter : public ImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiPrefabsPainter(EditorWorldSystem& system);
    };
} // namespace PJ
