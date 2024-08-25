#include "LoadRezOperation.h"
#include "LoadResourcesModel.h"
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
        PJLog("ERROR. Can't open file at: %s", info.filePath.c_str());
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

            LoadResourceInfo loadResourceInfo(fullPath, type, id);
            auto loadOperations = loadResourcesModel.MakeLoadOperations(loadResourceInfo);

            AddRange(result.loadOperations, loadOperations);
        }

        return result;
    } catch (...) {}

    return Failure();
}
