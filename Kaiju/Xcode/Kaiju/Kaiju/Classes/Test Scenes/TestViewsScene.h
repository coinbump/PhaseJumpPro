#pragma once

#include "SDKIncludes.h"

using namespace PJ;

class TestViewsScene : public Scene {
protected:
    View2D* RootView() const;

    void RebuildTargetView();

public:
    using BuildViewFunc = std::function<void(WorldNode&)>;

    struct LayoutModel {
        String id;
        Vector2 size;
    };

    VectorList<LayoutModel> layouts;
    WorldComponent* rootComponent{};

    struct BuildViewModel {
        String name;
        BuildViewFunc buildViewFunc;
    };

    VectorList<BuildViewModel> buildViewModels;

    int targetView{};

    TestViewsScene();

    std::optional<int> LayoutSizeIndex() const;
    void SetLayoutSizeIndex(int value);
    void SetLayoutSize(Vector2 value);

    void LoadInto(WorldNode& root) override;
};
