#include "StandardLoadResourcesModel.h"
#include "ResourceTypeEnum.h"
#include "SDLLoadGLTextureOperation.h"

using namespace std;
using namespace PJ;

StandardLoadResourcesModel::StandardLoadResourcesModel() {
    fileExtensionMap["png"] = "texture";
    fileExtensionMap["jpg"] = "texture";
    fileExtensionMap["jpeg"] = "texture";
    fileExtensionMap["bmp"] = "texture";

    auto allocator = []() -> SP<SomeLoadResourcesOperation> { return MAKE<SDLLoadGLTextureOperation>(); };
    auto factory = MAKE<Factory<SomeLoadResourcesOperation>>(allocator);
    operationRegistry["texture"] = factory;
}
