#include "ResourceRepository.h"
#include "Dev.h"
#include "FileManager.h"
#include "ResourceRepositoryModel.h"
#include "ResourceRepositoryPlan.h"
#include "SomeLoadResourcesOperation.h"
#include <filesystem>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

void ResourceRepository::Load(ResourceInfo info) {
    // Important: don't use this in a shipping app. This blocks the main thread
    auto operations = MakeLoadOperations(info);
    Run(operations);
}

void ResourceRepository::Run(VectorList<SP<SomeLoadResourcesOperation>> const& operations) {
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
                    for (auto& loadedResource : successValue->resources) {
                        resourceCatalog.Add(loadedResource);

                        PJ::Log(
                            "Loaded Resource: ", loadedResource.info.id,
                            " Type: ", loadedResource.info.type
                        );
                    }

                    // Loading a resource can kickstart new load resource operations
                    // Example: load metadata file + load image
                    // Currently we assume that this can only happen once
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

VectorList<SP<SomeLoadResourcesOperation>> ResourceRepository::MakeLoadOperations(ResourceInfo info
) {
    auto operations = repoModel.MakeLoadOperations(info);
    if (IsEmpty(operations)) {
        PJ::Log("ERROR. No load operations for type: ", info.type);
        return operations;
    }

    return operations;
}

VectorList<SP<SomeLoadResourcesOperation>>
ResourceRepository::MakeLoadOperations(ResourceRepositoryPlan plan) {
    VectorList<SP<SomeLoadResourcesOperation>> result;

    for (auto& info : plan.infos) {
        auto operations = MakeLoadOperations(info);
        AddRange(result, operations);
    }

    return result;
}
