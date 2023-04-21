#ifndef PJRENDERMODELBUILDER_H
#define PJRENDERMODELBUILDER_H

namespace PJ {
    class SomeShaderProgram;
    class RenderModel;
    class RenderMesh;
    class RenderMaterial;

    struct RenderModelBuilder {
        RenderModel Build(SomeShaderProgram& program, RenderMesh const& mesh, RenderMaterial const& material);
    };
}

#endif
