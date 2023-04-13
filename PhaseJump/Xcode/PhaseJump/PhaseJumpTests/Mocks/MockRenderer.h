#ifndef PJTESTMOCKRENDERER_H
#define PJTESTMOCKRENDERER_H

#include "SomeRenderer.h"
#include "VectorList.h"

namespace PJTest
{
    class MockRenderer : public SomeRenderer {
    public:
        VectorList<RenderIntoModel> renderHistory;

        virtual void RenderInto(RenderIntoModel model) {
            renderHistory.Add(model);
        }
    };
}

#endif
