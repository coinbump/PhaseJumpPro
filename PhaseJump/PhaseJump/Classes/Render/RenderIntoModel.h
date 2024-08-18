#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class SomeRenderContext;

    class RenderIntoModel {
    public:
        SomeRenderContext* renderContext;

        RenderIntoModel(SomeRenderContext* renderContext) :
            renderContext(renderContext) {}
    };
} // namespace PJ
