#pragma once

#include "SomeRenderContext.h"
#include "Vector2.h"
#include "VectorList.h"
#include "RenderContextModel.h"
#include "SomeRenderEngine.h"

using namespace PJ;

/*
 RATING: 5 stars
 Simple type, used for tests
 CODE REVIEW: 9/2/24
 */
namespace PJTest {
    class MockRenderEngine : public PJ::SomeRenderEngine {
        virtual void RenderStart(SomeRenderContext* context) {}
        virtual void RenderDraw(RenderDrawModel const& drawModel) {}

        virtual void EnableFeature(String featureId, bool isEnabled) {}

        virtual void ProjectionMatrixLoadOrthographic(Vector2 size) {}
        virtual void LoadTranslate(Vector3 value) {}

        virtual SP<SomeRenderContext> MakeTextureBuffer() { return {}; }
        virtual UnorderedSet<String> EnabledFeatures() { return {}; }

        /// Called once for each render pass
        virtual void RenderPassStart() {}
        virtual bool IsContextCleared(uint32_t id) { return {}; }
        virtual void SetIsContextCleared(uint32_t id, bool value) {}
    };
    
    class MockRenderContext : public PJ::SomeRenderContext
    {
    public:
        using Base = PJ::SomeRenderContext;

        Vector2 size;
        VectorList<RenderContextModel> renderHistory;
        MockRenderEngine renderEngine;
        
        MockRenderContext(Vector2 size = Vector2(400, 200)) : Base(renderEngine), size(size) {
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
