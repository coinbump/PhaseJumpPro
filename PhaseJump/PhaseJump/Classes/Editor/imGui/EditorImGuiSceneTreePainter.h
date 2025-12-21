#pragma once

#include "ImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    class WorldNode;
    class EditorWorldSystem;

    /// Draws scene tree imGui UI for editor
    class EditorImGuiSceneTreePainter : public ImGuiPainter {
    public:
        using OnInspectFunc = std::function<void(WorldNode&)>;

        EditorWorldSystem& system;
        OnInspectFunc onInspectFunc;

        void DrawSceneTree(EditorWorldSystem& system, WorldNode& node, String name);

    public:
        EditorImGuiSceneTreePainter(EditorWorldSystem& system, OnInspectFunc onInspectFunc);
    };
} // namespace PJ
