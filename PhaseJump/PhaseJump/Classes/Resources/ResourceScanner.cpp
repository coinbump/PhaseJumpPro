#include "ResourceScanner.h"
#include "Dev.h"
#include "FileManager.h"
#include "ResourceRepositoryModel.h"
#include "ResourceRepositoryPlan.h"
#include "SomeLoadResourcesOperation.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

ResourceRepositoryPlan ResourceScanner::ScanAt(FilePath path, FileSearchType searchType) {
    ResourceRepositoryPlan result;

    // This doesn't do anything meaningful, but it's a first test of async code
    auto pathsFuture = fm.PathListAsync(path, searchType);
    pathsFuture.wait();
    auto paths = pathsFuture.get();

    for (auto& path : paths) {
        auto info = ScanFile(path);
        GUARD_CONTINUE(info)

        auto resourceInfo = info.value();
        result.infos.push_back(resourceInfo);
    }

    return result;
}

std::optional<ResourceInfo> ResourceScanner::ScanFile(FilePath path) {
    if (fm.IsDirectory(path)) {
        return std::nullopt;
    }

    auto fileExtension = fm.FileExtension(path, false);
    auto type = scanModel.TypeForFileExtension(fileExtension);

    // Unrecognized file extension
    GUARDR(type, {})

    ResourceInfo result;
    result.type = *type;
    result.id = fm.FileName(path, false);
    result.filePath = path;

    return result;
}

// MARK: StandardResourceScanModel

StandardResourceScanModel::StandardResourceScanModel(LoadType loadType) {
    switch (loadType) {
    case LoadType::Rez:
        {
            // Scan for .rez files, which describe the resources to be loaded
            fileExtensionMap[ResourceType::Rez] = ResourceType::Rez;
            break;
        }
    case LoadType::Scan:
        {
            // Scan for resource files (complex resource types not supported)
            fileExtensionMap["png"] = ResourceType::Texture;
            fileExtensionMap["jpg"] = ResourceType::Texture;
            fileExtensionMap["jpeg"] = ResourceType::Texture;
            fileExtensionMap["bmp"] = ResourceType::Texture;
            fileExtensionMap["shprog"] = ResourceType::ShaderProgram;
            break;
        }
    }
}
