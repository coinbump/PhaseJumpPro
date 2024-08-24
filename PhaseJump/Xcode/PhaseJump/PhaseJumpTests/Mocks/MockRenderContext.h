#ifndef PJTESTMOCKRENDERCONTEXT_H
#define PJTESTMOCKRENDERCONTEXT_H

#include "SomeRenderContext.h"
#include "Vector2.h"
#include "VectorList.h"
#include "RenderContextModel.h"

using namespace PJ;

namespace PJTest {

        class MockRenderContext : public PJ::SomeRenderContext
    {
    public:
        Vector2 size;
        VectorList<RenderContextModel> renderHistory;

        MockRenderContext(Vector2 size = Vector2(400, 200)) : size(size) {
        }

        // Make context current, for renders
        void Bind() override {}
        void Clear() override {}
        void Present() override {}

        Vector2 Size() const override
        {
            return size;
        }

        Vector2Int PixelSize() const override
        {
            return Vector2Int(Size().x / 2.0f, Size().y / 2.0f);
        }

        void Render(SomeRenderer& renderer, RenderContextModel model) override;
    };
}

#endif
