#include "StandardResourceRepositoryModel.h"
#include "GLLoadShaderProgramOperation.h"
#include "LoadBitmapFontOperation.h"
#include "LoadRezOperation.h"
#include "LoadRTexPackerAtlasOperation.h"
#include "LoadTexturePackerAtlasOperation.h"
#include "SDLLoadGLTextureOperation.h"

using namespace std;
using namespace PJ;

StandardResourceRepositoryModel::StandardResourceRepositoryModel() {
    operationRegistry.map[ResourceType::ShaderProgram] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<GLLoadShaderProgramOperation>(info, repoModel);
            }
        );
    operationRegistry.map[ResourceType::Texture] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<SDLLoadGLTextureOperation>(info, repoModel);
            }
        );
    operationRegistry.map[ResourceType::RTexPackerAtlas] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<LoadRTexPackerAtlasOperation>(info, repoModel);
            }
        );
    operationRegistry.map[ResourceType::TexturePackerAtlas] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<LoadTexturePackerAtlasOperation>(info, repoModel);
            }
        );
    operationRegistry.map[ResourceType::BitmapFont] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<LoadBitmapFontOperation>(info, repoModel);
            }
        );
    operationRegistry.map[ResourceType::Rez] =
        NEW<Factory<SomeLoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>>(
            [](ResourceInfo info, ResourceRepositoryModel& repoModel) {
                return NEW<LoadRezOperation>(info, repoModel);
            }
        );
}
