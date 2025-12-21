#pragma once

#include "GLShaderProgram.h"
#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class GLShaderProgram;

    /// Loads OpenGL shader program
    class GLLoadShaderProgramOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        GLLoadShaderProgramOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
