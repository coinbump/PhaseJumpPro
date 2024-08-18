#include "MeshRendererModel.h"

using namespace std;
using namespace PJ;

void MeshRendererModel::SetMeshBuilderFunc(MeshBuilderFuncType meshBuilderFunc) {
    this->meshBuilderFunc = meshBuilderFunc;
    GUARD(meshBuilderFunc)
    mesh = meshBuilderFunc(worldSize);
}

void MeshRendererModel::SetWorldSize(Vector3 value) {
    this->worldSize = value;

    GUARD(meshBuilderFunc)
    mesh = meshBuilderFunc(worldSize);
}
