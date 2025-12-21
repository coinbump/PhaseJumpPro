#pragma once

#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    class GLRenderEngine;

    /// Interface for OpenGL render command
    class SomeGLRenderCommand {
    public:
        virtual ~SomeGLRenderCommand() {}

        virtual void Run(GLRenderEngine& engine) = 0;
    };

    /// OpenGL render command
    template <class Core>
    struct GLRenderCommand : public SomeGLRenderCommand {
        using This = GLRenderCommand<Core>;

        using RunFunc = std::function<void(This&, GLRenderEngine&)>;

        Core core{};
        RunFunc runFunc;

        GLRenderCommand(Core const& core, RunFunc runFunc) :
            core(core),
            runFunc(runFunc) {}

        void Run(GLRenderEngine& engine) override {
            GUARD(runFunc)
            runFunc(*this, engine);
        }
    };

    namespace GLRenderCommands {
        extern UP<SomeGLRenderCommand> SetViewportRender(Vector2Int size);
        extern UP<SomeGLRenderCommand> ProjectionMatrixLoadOrthographic(Vector2 size);
        extern UP<SomeGLRenderCommand> LoadTranslate(Vector3 value);
    } // namespace GLRenderCommands
} // namespace PJ
