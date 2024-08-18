#include "ResourceScanner.h"
#include "FileManager.h"
#include "LoadResourcesModel.h"
#include "LoadResourcesPlan.h"
#include "Log.h"
#include "SomeLoadResourcesOperation.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

LoadResourcesPlan ResourceScanner::ScanAt(FilePath path, bool isRecursive) {
    LoadResourcesPlan result;

    if (nullptr == fm || nullptr == loadResourcesModel) {
        return result;
    }
    auto& fm = (*this->fm);

    // This doesn't do anything meaningful, but it's a first test of async code
    auto pathsFuture = fm.PathListAsync(path, isRecursive);
    pathsFuture.wait();
    auto paths = pathsFuture.get();

    for (auto& path : paths) {
        auto info = ScanFile(path);
        if (info) {
            auto resourceInfo = info.value();
            result.infoMap[resourceInfo.type][resourceInfo.id] = resourceInfo;
        }
    }

    return result;
}

std::optional<LoadResourceInfo> ResourceScanner::ScanFile(FilePath path) {
    if (nullptr == fm || nullptr == loadResourcesModel) {
        return std::nullopt;
    }
    auto& fm = (*this->fm);

    if (fm.IsDirectory(path)) {
        return std::nullopt;
    }

    LoadResourceInfo result;

    auto fileExtension = fm.FileExtension(path, false);

    auto type = loadResourcesModel->TypeIdForFileExtension(fileExtension);
    if (std::nullopt == type) {
        // PJLog("WARNING. Unrecognized resource extension, %s", fileExtension.c_str());
        return std::nullopt;
    }

    result.type = type.value();

    result.id = fm.FileName(path, false);
    result.filePath = path;

    return result;
}
