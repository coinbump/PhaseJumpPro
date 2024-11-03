#pragma once

#include "SomeImGuiPainter.h"

namespace PJ {
    class EditorWorldSystem;

    /// Draws list of world resources, with info and tools
    class EditorImGuiPrefabsPainter : public SomeImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiPrefabsPainter(EditorWorldSystem& system);
    };
} // namespace PJ
