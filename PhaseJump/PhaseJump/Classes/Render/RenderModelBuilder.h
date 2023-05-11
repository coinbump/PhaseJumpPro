#ifndef PJRENDERMODELBUILDER_H
#define PJRENDERMODELBUILDER_H

namespace PJ {
    class SomeShaderProgram;
    class RenderModel;
    class RenderMesh;
    class RenderMaterial;
    class Matrix4x4;

    struct RenderModelBuilder {
        RenderModel Build(SomeShaderProgram& program, RenderMesh const& mesh, RenderMaterial const& material, Matrix4x4 modelMatrix, float z);
    };
}

#endif
