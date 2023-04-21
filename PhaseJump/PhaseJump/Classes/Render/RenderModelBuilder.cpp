#include "RenderModelBuilder.h"
#include "RenderModel.h"
#include "RenderMesh.h"
#include "RenderMaterial.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

RenderModel RenderModelBuilder::Build(SomeShaderProgram& program, RenderMesh const& mesh, RenderMaterial const& material) {
    RenderModel renderModel(program);
    renderModel.vertices = mesh.vertices;
    renderModel.indices = mesh.triangles;
    renderModel.uvs = mesh.uvs;
    renderModel.colors = material.colors;
    renderModel.uniformColors = material.uniformColors;
    renderModel.uniformFloats = material.uniformFloats;
    renderModel.features = material.features;
    renderModel.textures = material.textures;

    return renderModel;
}
