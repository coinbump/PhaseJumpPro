#include "CanvasAppScene.h"

using namespace std;
using namespace PJ;

void CanvasAppScene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Canvas App")
        .OrthoStandard()
        .With<ObjectCanvas>()
        .SizeWithWindow()
        .ModifyLatest<ObjectCanvas>([](auto& canvas) {
            canvas.SetActiveTool(MAKE<SelectObjectsCanvasTool>());
        });
}
