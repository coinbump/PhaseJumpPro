#ifndef PJRENDERINTOMODEL_H
#define PJRENDERINTOMODEL_H

#include "Vector3.h"
#include <memory>

namespace PJ
{
    class SomeRenderContext;

    class RenderIntoModel
    {
    public:
        std::shared_ptr<SomeRenderContext> renderContext;
        Vector3 position;
    };
}

#endif
