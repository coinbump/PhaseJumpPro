#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class SomeGLShader;

    /// Loads OpenGL shader
    class GLLoadShaderOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;
        using FactoryFunc = std::function<SP<SomeGLShader>()>;

        FactoryFunc factoryFunc;

        GLLoadShaderOperation(
            ResourceInfo info, ResourceRepositoryModel& repoModel, FactoryFunc factoryFunc
        ) :
            Base(info, repoModel),
            factoryFunc(factoryFunc) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
