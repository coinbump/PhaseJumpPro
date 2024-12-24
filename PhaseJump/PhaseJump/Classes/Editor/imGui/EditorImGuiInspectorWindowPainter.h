#pragma once

#include "EditorWorldSystem.h"
#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /// Draws an inspector window for the currently inspected world node and edit UI for the scene
    class EditorImGuiInspectorWindowPainter : public SomeImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiInspectorWindowPainter(EditorWorldSystem& _system);
    };
} // namespace PJ
