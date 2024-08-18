#pragma once

#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 7/16/24
 */
namespace PJ {
    // FUTURE: add commands as needed
    class GLRenderEngine;

    /// Interface for OpenGL render command
    struct SomeGLRenderCommand {
        virtual ~SomeGLRenderCommand() {}

        virtual void Run(GLRenderEngine& engine) = 0;
    };

    /// OpenGL render command
    template <class Core>
    struct GLRenderCommand : public SomeGLRenderCommand {
        using RunFunc = std::function<void(Core&, GLRenderEngine&)>;

        Core core;
        RunFunc runFunc;

        GLRenderCommand(Core core, RunFunc runFunc) :
            core(core),
            runFunc(runFunc) {}

        void Run(GLRenderEngine& engine) override {
            GUARD(runFunc)
            runFunc(core, engine);
        }
    };

    namespace GLRenderCommands {
        extern SP<SomeGLRenderCommand> SetViewportRender(Vector2Int size);
        extern SP<SomeGLRenderCommand> ProjectionMatrixLoadOrthographic(Vector2 size);
        extern SP<SomeGLRenderCommand> LoadTranslate(Vector3 value);
    } // namespace GLRenderCommands
} // namespace PJ
