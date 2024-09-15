#pragma once

#include "SomeRenderContext.h"
#include "Vector2.h"
#include "VectorList.h"
#include "RenderContextModel.h"

using namespace PJ;

/*
 RATING: 5 stars
 Simple type, used for tests
 CODE REVIEW: 9/2/24
 */
namespace PJTest {
    class MockRenderContext : public PJ::SomeRenderContext
    {
    public:
        using Base = PJ::SomeRenderContext;

        Vector2 size;
        VectorList<RenderContextModel> renderHistory;

        MockRenderContext(Vector2 size = Vector2(400, 200)) : Base(nullptr), size(size) {
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
            return Vector2Int(size.x, size.y);
        }
    };
}
