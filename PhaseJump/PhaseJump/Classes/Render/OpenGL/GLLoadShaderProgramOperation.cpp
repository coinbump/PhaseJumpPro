#include "GLLoadShaderProgramOperation.h"
#include "GLLoadShaderOperation.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "StandardResourceRepositoryModel.h"
#include <fstream>
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

std::optional<GLShaderProgram::Info> InfoFromPath(FilePath path) {
    GLShaderProgram::Info result;

    String progString;
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        PJ::Log("ERROR. Can't open .shprog file at: ", path);
        return {};
    }

    try {
        json j;
        file >> j;

        String _vertexShaderPath = j["shader.vertex.path"];
        String _fragmentShaderPath = j["shader.fragment.path"];

        FilePath basePath = path;
        basePath.remove_filename();

        FilePath vertexShaderPath = basePath;
        vertexShaderPath /= _vertexShaderPath;
        FilePath fragmentShaderPath = basePath;
        fragmentShaderPath /= _fragmentShaderPath;

        result.id = j["id"];
        result.vertexShaderPath = vertexShaderPath;
        result.fragmentShaderPath = fragmentShaderPath;
    } catch (...) {
        PJ::Log("ERROR: Couldn't parse GLShaderProgram JSON");
        return {};
    }

    return result;
}

LoadResourcesOperation::Result GLLoadShaderProgramOperation::_LoadResources() {
    auto info = InfoFromPath(this->info.filePath);
    GUARDR(info, Failure())

    ResourceRepositoryModel repositoryModel;

    SP<VertexGLShader> vertexShader;
    SP<FragmentGLShader> fragmentShader;

    {
        GLLoadShaderOperation loader({ .filePath = info->vertexShaderPath }, repositoryModel, []() {
            return MAKE<VertexGLShader>();
        });
        loader.Run();

        auto resources = loader.Resources();
        GUARDR(!IsEmpty(resources), Failure());

        vertexShader = DCAST<VertexGLShader>(resources[0].resource);
        GUARDR(vertexShader, Failure())
    }

    {
        GLLoadShaderOperation loader(
            { .filePath = info->fragmentShaderPath }, repositoryModel,
            []() { return MAKE<FragmentGLShader>(); }
        );
        loader.Run();

        auto resources = loader.Resources();
        GUARDR(!IsEmpty(resources), Failure());

        fragmentShader = DCAST<FragmentGLShader>(resources[0].resource);
        GUARDR(fragmentShader, Failure())
    }

    auto result = MAKE<GLShaderProgram>();
    result->Configure(vertexShader, fragmentShader);

    ShaderProgram::registry[info->id] = result;

    ResourceModel resource{ .info = this->info, .resource = result };
    VectorList<ResourceModel> resources{ resource };

    return Result(Success(resources, {}));
}
