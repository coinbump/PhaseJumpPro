#include "ResourceRepository.h"
#include "FileManager.h"
#include "LoadResourcesModel.h"
#include "LoadResourcesPlan.h"
#include "Log.h"
#include "SomeLoadResourcesOperation.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

void ResourceRepository::Load(LoadResourceInfo info) {
    if (nullptr == loadResourcesModel) {
        return;
    }

    auto operations = LoadOperations(info);
    Run(operations);
}

void ResourceRepository::Run(List<SP<SomeLoadResourcesOperation>> const& operations) {
    // FUTURE: support loading on background thread
    // FUTURE: support both main + background operations (load image on
    // background thread -> create OpenGL texture on main thread)
    for (auto& operation : operations) {
        operation->Run();
        GUARD_CONTINUE(operation->result)

        switch (operation->result->Type()) {
        case ResultType::Success:
            {
                auto successValue = operation->result->SuccessValue();
                if (successValue) {
                    for (auto& loadedResource : successValue->loadedResources) {
                        loadedResources->map[loadedResource.type][loadedResource.id] =
                            loadedResource;

                        PJLog(
                            "Loaded Resource: %s\t Type: %s", loadedResource.id.c_str(),
                            loadedResource.type.c_str()
                        );
                    }

                    auto& loadOperations = (*successValue).loadOperations;
                    Run(loadOperations);
                }
                break;
            }
        case ResultType::Failure:
            PJLog("ERROR. Failed to load resource");
            break;
        }
    }
}

List<SP<SomeLoadResourcesOperation>> ResourceRepository::LoadOperations(LoadResourceInfo info) {
    auto operations = loadResourcesModel->MakeLoadOperations(info);
    if (IsEmpty(operations)) {
        PJLog("ERROR. No load operations for type: %s", info.type.c_str());
        return operations;
    }

    return operations;
}

List<SP<SomeLoadResourcesOperation>> ResourceRepository::LoadOperations(LoadResourcesPlan plan) {
    List<SP<SomeLoadResourcesOperation>> result;

    auto allInfos = plan.AllInfos();
    for (auto& info : allInfos) {
        auto operations = LoadOperations(info);
        AddRange(result, operations);
    }

    return result;
}
