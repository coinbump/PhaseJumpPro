#pragma once

#include "EditorWorldSystem.h"
#include "ImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /// Draws an inspector window for the currently inspected world node and edit UI for the scene
    class EditorImGuiInspectorWindowPainter : public ImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiInspectorWindowPainter(EditorWorldSystem& _system);
    };
} // namespace PJ
