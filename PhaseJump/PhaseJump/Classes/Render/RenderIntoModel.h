#ifndef PJRENDERINTOMODEL_H
#define PJRENDERINTOMODEL_H

#include "Vector3.h"
#include <memory>
#include "Matrix4x4.h"

namespace PJ
{
    class SomeRenderContext;

    class RenderIntoModel
    {
    public:
        std::shared_ptr<SomeRenderContext> renderContext;
        Matrix4x4 modelMatrix;
    };
}

#endif
