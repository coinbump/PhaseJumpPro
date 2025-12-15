#include "ResourceRepositoryModel.h"

using namespace std;
using namespace PJ;

using LoadResourcesOperationList = ResourceRepositoryModel::LoadResourcesOperationList;

std::optional<String> ResourceRepositoryModel::TypeForFileExtension(String fileExtension) {
    auto iterator = fileExtensionMap.find(fileExtension);
    GUARDR(iterator != fileExtensionMap.end(), std::nullopt)

    return iterator->second;
}

LoadResourcesOperationList ResourceRepositoryModel::MakeLoadOperations(ResourceInfo info) {
    LoadResourcesOperationList result;

    // FUTURE: support multiple load operations if needed
    auto operation = operationRegistry.Make(info.type, info, *this);
    GUARDR(operation, result)

    Add(result, operation);
    return result;
}
