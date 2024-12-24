#pragma once

#include "GLShaderProgram.h"
#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class GLShaderProgram;

    /// Loads OpenGL shader program
    class GLLoadShaderProgramOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        GLLoadShaderProgramOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
