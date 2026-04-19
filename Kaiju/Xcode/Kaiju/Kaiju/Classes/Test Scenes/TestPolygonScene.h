#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Abstract component for gestures/logic to handle dragging
class TestPolygonScene : public Scene {
public:
    using This = TestPolygonScene;

    std::function<UIWorldSystem*()> uiSystemResolver;

    virtual UIWorldSystem* UISystem() const {
        GUARDR(uiSystemResolver, nullptr)
        return uiSystemResolver();
    }

    Polygon polygon;

    // MARK: Scene

    void LoadInto(WorldNode& root) override;

protected:
    void OnPointerDown(PointerDownUIEvent const& event);

public:
    TestPolygonScene();
};
