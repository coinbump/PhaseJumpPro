#pragma once

#include "ImGuiPainter.h"
#include "Scene.h"
#include <functional>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/21/24
 */
namespace PJ {
    class World;
    class EditorWorldSystem;

    /// Draws a list of scenes to navigate to for the editor
    class EditorImGuiScenesPainter : public ImGuiPainter {
    public:
        using OnSceneSwitchFunc = std::function<void(Scene&)>;

        EditorWorldSystem& system;
        OnSceneSwitchFunc onSceneSwitchFunc;

        EditorImGuiScenesPainter(EditorWorldSystem& system, OnSceneSwitchFunc onSceneSwitchFunc);
    };
} // namespace PJ
