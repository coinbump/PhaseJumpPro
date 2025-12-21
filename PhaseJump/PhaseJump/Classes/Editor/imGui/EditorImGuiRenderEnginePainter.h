#pragma once

#include "EditorWorldSystem.h"
#include "ImGuiPainter.h"
#include "RenderProcessingModel.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /// Draws information about the render engine and render processors for the editor
    class EditorImGuiRenderEnginePainter : public ImGuiPainter {
    public:
        EditorWorldSystem& system;

        EditorImGuiRenderEnginePainter(EditorWorldSystem& _system);
    };

    /// Draws information about the render processor pipeline
    class EditorImGuiRenderPipelinePainter : public ImGuiPainter {
    public:
        World& world;
        RenderProcessingModel::ProcessorList& processors;

        EditorImGuiRenderPipelinePainter(
            World& world, RenderProcessingModel::ProcessorList& processors
        );
    };
} // namespace PJ
