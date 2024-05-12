#ifndef PJRENDERMODELBUILDER_H
#define PJRENDERMODELBUILDER_H

namespace PJ {
    class SomeShaderProgram;
    struct RenderModel;
    struct Mesh;
    class RenderMaterial;
    class Matrix4x4;

    struct RenderModelBuilder {
        RenderModel Build(SomeShaderProgram& program, Mesh const& mesh, RenderMaterial const& material, Matrix4x4 modelMatrix, float z);
    };
}

#endif
