#include "GLShaderProgramLoader.h"
#include "GLShaderLoader.h"
#include "GLShaderProgram.h"
#include "SomeGLShader.h"
#include <fstream>
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

SP<GLShaderProgram>
GLShaderProgramLoader::LoadFromShaderPaths(FilePath vertexShaderPath, FilePath fragmentShaderPath) {
    GLShaderLoader loader;
    auto vertexShader = MAKE<VertexGLShader>();
    if (!loader.LoadFromPath(*vertexShader, vertexShaderPath)) {
        return nullptr;
    }

    auto fragmentShader = MAKE<FragmentGLShader>();
    if (!loader.LoadFromPath(*fragmentShader, fragmentShaderPath)) {
        return nullptr;
    }

    auto result = MAKE<GLShaderProgram>();
    result->Configure(vertexShader, fragmentShader);
    return result;
}

std::optional<GLShaderProgram::Info> GLShaderProgramLoader::InfoFromPath(FilePath path) {
    GLShaderProgram::Info result;

    String progString;
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        PJ::Log("ERROR. Can't open .shprog file at: ", path);
        return std::nullopt;
    }

    try {
        json j;
        file >> j;

        String vertexShaderName = j["shader.vertex"];
        String fragmentShaderName = j["shader.fragment"];

        String vertexShaderExtension = ".vsh";
        String fragmentShaderExtension = ".fsh";

        String vertexShaderFileName = vertexShaderName + vertexShaderExtension;
        String fragmentShaderFileName = fragmentShaderName + fragmentShaderExtension;

        FilePath basePath = path;
        basePath.remove_filename();

        FilePath vertexShaderPath = basePath;
        vertexShaderPath /= "vertex";
        vertexShaderPath /= FilePath(vertexShaderFileName.c_str());
        FilePath fragmentShaderPath = basePath;
        fragmentShaderPath /= "fragment";
        fragmentShaderPath /= FilePath(fragmentShaderFileName.c_str());

        result.id = j["id"];
        result.vertexShaderPath = vertexShaderPath;
        result.fragmentShaderPath = fragmentShaderPath;
    } catch (...) {
        PJ::Log("ERROR: Couldn't parse GLShaderProgram JSON");
        return std::nullopt;
    }

    return std::make_optional(result);
}
