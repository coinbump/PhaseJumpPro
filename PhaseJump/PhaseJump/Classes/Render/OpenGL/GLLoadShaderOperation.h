#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class GLShader;

    /// Loads OpenGL shader
    class GLLoadShaderOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;
        using FactoryFunc = std::function<SP<GLShader>()>;

        FactoryFunc factoryFunc;

        GLLoadShaderOperation(
            ResourceInfo info, ResourceRepositoryModel& repoModel, FactoryFunc factoryFunc
        );

        Result _LoadResources();
    };
} // namespace PJ
