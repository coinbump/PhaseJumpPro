#include "RendererModel.h"

using namespace std;
using namespace PJ;

void RendererModel::SetMeshBuilderFunc(MeshBuilderFuncType value) {
    meshBuilderFunc = value;
    meshNeedsBuild = true;
}

void RendererModel::SetWorldSize(Vector3 value) {
    GUARD(worldSize != value)
    worldSize = value;
    meshNeedsBuild = true;
}
