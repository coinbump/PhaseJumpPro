#pragma once

#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class EditorWorldSystem;

    /// Draws list of world resources, with info and tools
    class EditorImGuiResourcesPainter : public SomeImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiResourcesPainter(EditorWorldSystem& system);
    };
} // namespace PJ
