#include "StandardLoadResourcesModel.h"
#include "LoadRezOperation.h"
#include "LoadRTexPackerAtlasOperation.h"
#include "LoadTexturePackerAtlasOperation.h"
#include "ResourceTypeEnum.h"
#include "SDLLoadGLTextureOperation.h"

using namespace std;
using namespace PJ;

StandardLoadResourcesModel::StandardLoadResourcesModel(LoadType loadType) {
    switch (loadType) {
    case LoadType::Rez:
        {
            // Scan for .rez files, which describe the resources to be loaded
            fileExtensionMap["rez"] = "rez";
            break;
        }
    case LoadType::Scan:
        {
            // Scan for resource files (complex resource types not supported)
            fileExtensionMap["png"] = "texture";
            fileExtensionMap["jpg"] = "texture";
            fileExtensionMap["jpeg"] = "texture";
            fileExtensionMap["bmp"] = "texture";
            break;
        }
    }

    operationRegistry.map["texture"] =
        MAKE<Factory<SomeLoadResourcesOperation, LoadResourceInfo, LoadResourcesModel&>>(
            [](LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) {
                return MAKE<SDLLoadGLTextureOperation>(info, loadResourcesModel);
            }
        );
    operationRegistry.map["rtexPacker.atlas"] =
        MAKE<Factory<SomeLoadResourcesOperation, LoadResourceInfo, LoadResourcesModel&>>(
            [](LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) {
                return MAKE<LoadRTexPackerAtlasOperation>(info, loadResourcesModel);
            }
        );
    operationRegistry.map["texturePacker.atlas"] =
        MAKE<Factory<SomeLoadResourcesOperation, LoadResourceInfo, LoadResourcesModel&>>(
            [](LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) {
                return MAKE<LoadTexturePackerAtlasOperation>(info, loadResourcesModel);
            }
        );
    operationRegistry.map["rez"] =
        MAKE<Factory<SomeLoadResourcesOperation, LoadResourceInfo, LoadResourcesModel&>>(
            [](LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) {
                return MAKE<LoadRezOperation>(info, loadResourcesModel);
            }
        );
}
