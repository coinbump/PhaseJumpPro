#include "ResourceRepository.h"
#include "LoadResourcesPlan.h"
#include "FileManager.h"
#include "SomeLoadResourcesOperation.h"
#include "SomeLoadResourcesModel.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

LoadResourcesPlan ResourceRepository::Scan(FilePath path, bool isRecursive) {
    LoadResourcesPlan result;

    if (nullptr == fm || nullptr == loadResourcesModel) { return result; }
    auto & fm = (*this->fm);

    auto paths = fm.PathList(path, isRecursive);
    for (auto path : paths) {
        auto info = ScanFile(path);
        if (info) {
            auto resourceInfo = info.value();
            result.infoMap[resourceInfo.typeId][resourceInfo.id] = resourceInfo;
        }
    }

    return result;
}

std::optional<LoadResourceInfo> ResourceRepository::ScanFile(FilePath path) {
    if (nullptr == fm || nullptr == loadResourcesModel) { return std::nullopt; }
    auto & fm = (*this->fm);

    if (fm.IsDirectory(path)) { return std::nullopt; }

    LoadResourceInfo result;

    auto fileExtension = fm.FileExtension(path);
    fileExtension.RemoveIf([] (char c) { return c == '.'; });

    result.typeId = loadResourcesModel->fileExtensionMap[fileExtension];

    if (result.typeId.IsEmpty()) {
        PJLog("WARNING. Unrecognized resource extension, %s", fileExtension.c_str());
    }

    result.id = fm.FileName(path, false);
    result.filePath = path;

    return result;
}

void ResourceRepository::Load(LoadResourceInfo info) {
    if (nullptr == loadResourcesModel) { return; }

    auto operation = LoadOperation(info);
    if (nullptr == operation) { return; }

    operation->Run();
    if (nullptr == operation->result) { return; }

    switch (operation->result->Type()) {
        case ResultType::Success: {
            auto successValue = operation->result->SuccessValue();

            for (auto loadedResource : successValue) {
                loadedResources->map[loadedResource.typeId][loadedResource.id] = loadedResource;
            }

            PJLog("Loaded Resource: %s", info.filePath.filename().string().c_str());
            break;
        }
        case ResultType::Failure:
            PJLog("ERROR. Failed to load resource: %s", info.filePath.filename().string().c_str());
            break;
    }
}

SP<SomeLoadResourcesOperation> ResourceRepository::LoadOperation(LoadResourceInfo info) {
    auto operation = loadResourcesModel->operationRegistry.New(info.typeId);
    if (nullptr == operation) {
        PJLog("ERROR. No load operation for type: %s", info.typeId.c_str());
        return operation;
    }

    operation->info = info;

    return operation;
}

VectorList<SP<SomeLoadResourcesOperation>> ResourceRepository::LoadOperations(LoadResourcesPlan plan) {
    VectorList<SP<SomeLoadResourcesOperation>> result;

    auto allInfos = plan.AllInfos();
    for (auto info : allInfos) {
        auto operation = LoadOperation(info);
        if (operation) {
            result.Add(operation);
        }
    }

    return result;
}
