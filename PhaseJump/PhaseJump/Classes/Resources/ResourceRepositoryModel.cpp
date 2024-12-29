#include "ResourceRepositoryModel.h"

using namespace std;
using namespace PJ;

std::optional<String> ResourceRepositoryModel::TypeForFileExtension(String fileExtension) {
    auto iterator = fileExtensionMap.find(fileExtension);
    GUARDR(iterator != fileExtensionMap.end(), std::nullopt)

    return iterator->second;
}

VectorList<SP<SomeLoadResourcesOperation>>
ResourceRepositoryModel::MakeLoadOperations(ResourceInfo info) {
    VectorList<SP<SomeLoadResourcesOperation>> result;

    // FUTURE: support multiple load operations if needed
    auto operation = operationRegistry.Make(info.type, info, *this);
    GUARDR(operation, result)

    Add(result, operation);
    return result;
}
