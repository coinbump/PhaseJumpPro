#include "ResourceRepository.h"
#include "Dev.h"
#include "FileManager.h"
#include "LoadResourcesModel.h"
#include "LoadResourcesPlan.h"
#include "SomeLoadResourcesOperation.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

void ResourceRepository::Load(ResourceInfo info) {
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
                        loadedResources->map[loadedResource.info.type][loadedResource.info.id] =
                            loadedResource;

                        PJ::Log(
                            "Loaded Resource: ", loadedResource.info.id,
                            " Type: ", loadedResource.info.type
                        );
                    }

                    auto& loadOperations = (*successValue).loadOperations;
                    Run(loadOperations);
                }
                break;
            }
        case ResultType::Failure:
            PJ::Log("ERROR. Failed to load resource");
            break;
        }
    }
}

List<SP<SomeLoadResourcesOperation>> ResourceRepository::LoadOperations(ResourceInfo info) {
    auto operations = loadResourcesModel->MakeLoadOperations(info);
    if (IsEmpty(operations)) {
        PJ::Log("ERROR. No load operations for type: ", info.type);
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
