#ifndef PJTESTMOCKRENDERER_H
#define PJTESTMOCKRENDERER_H

#include "SomeRenderer.h"
#include "VectorList.h"

namespace PJTest
{
    class MockRenderer : public SomeRenderer {
    public:
        VectorList<RenderIntoModel> renderHistory;
        
        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) override {
            VectorList<RenderModel> result;

            renderHistory.Add(model);

            return result;
        }

        void RenderInto(RenderIntoModel const& model) override {
        }
    };
}

#endif
