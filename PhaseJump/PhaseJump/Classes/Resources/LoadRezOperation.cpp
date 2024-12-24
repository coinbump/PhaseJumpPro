#include "LoadRezOperation.h"
#include "ResourceRepositoryModel.h"
#include <fstream>
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

SomeLoadResourcesOperation::Result LoadRezOperation::LoadResources() {
    FilePath filePath = info.filePath;

    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        PJ::Log("ERROR. Can't open file at: ", info.filePath);
        return Failure();
    }

    try {
        json j;
        file >> j;

        List<json::value_type> resources = j["resources"];

        Success result;

        for (auto& resource : resources) {
            String type = resource["type"];
            String pathString = resource["path"];
            FilePath resourcePath(pathString.c_str());

            String id;
            try {
                id = resource["id"];
            } catch (...) {
                FilePath idPath = resourcePath;
                idPath.replace_extension().string();
                id = idPath.filename().string();
            }

            FilePath fullPath = info.filePath;
            fullPath.remove_filename();
            fullPath /= resourcePath;

            ResourceInfo loadResourceInfo{ .id = id, .filePath = fullPath, .type = type };
            auto loadOperations = repoModel.MakeLoadOperations(loadResourceInfo);

            AddRange(result.loadOperations, loadOperations);
        }

        return result;
    } catch (...) {}

    return Failure();
}
