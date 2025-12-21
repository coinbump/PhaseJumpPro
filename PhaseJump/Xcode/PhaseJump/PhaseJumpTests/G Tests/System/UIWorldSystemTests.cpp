#include "gtest/gtest.h"
#include "UIWorldSystem.h"
#include "MockRenderContext.h"
#include "World.h"
#include "DragHandler.h"
#include "Camera.h"

using namespace PJ;
using namespace PJTest;
using namespace std;

namespace UIWorldSystemTests {
    class TestDragHandler : public DragHandler {
    public:
        WorldPosition lastPosition{ -1000, -1000, -1000 };
        
        void OnDragUpdate(WorldPosition inputPosition) override {
            lastPosition = inputPosition;
        }
    };
    
    class TestUIWorldSystem : public UIWorldSystem {
    public:
        SP<PJ::World> world;
        Camera* camera{};

        TestUIWorldSystem(MockRenderContext::Config mockRenderContextConfig) {
            world = MAKE<PJ::World>();
            auto _renderContext = NEW<MockRenderContext>(mockRenderContextConfig);
            world->renderContext = std::move(_renderContext);

            camera = &world->AddNode().AddComponent<OrthoCamera>();
        }
        
        Camera* GetCamera() const override {
            return camera;
        }
    };
    
    class MockMouseDevice : public SomeMouseDevice {
    public:
        ScreenPosition screenPos{200, 100};
        
        MockMouseDevice(ScreenPosition screenPos) : screenPos(screenPos) {
        }
        
        bool IsAvailable() override {
            return true;
        }
        
        ScreenPosition GetScreenPosition() const override {
            return screenPos;
        }
        
        bool IsButtonDown(PointerInputButtonType button) override {
            return true;
        }
    };
}

using namespace UIWorldSystemTests;

// FUTURE: add more tests
TEST(UIWorldSystem, DragUpdatePosition) {
    TestUIWorldSystem sut(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    SP<SomeMouseDevice> mouseDevice = MAKE<MockMouseDevice>(ScreenPosition{200, 100});
    sut.mouseDevice = mouseDevice;
    
    auto dragHandler = MAKE<TestDragHandler>();
    auto dragModel = MAKE<DragModel>(dragHandler);
    sut.StartDrag(dragModel);
    
    EXPECT_TRUE(sut.IsDragging());
    
    // Calls OnDragUpdate
    sut.OnUpdate({0});
    
    EXPECT_EQ(Vector3(0, 0, 0), dragHandler->lastPosition);
}

TEST(UIWorldSystem, DragUpdatePositionScaled) {
    TestUIWorldSystem sut(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    SP<SomeMouseDevice> mouseDevice = MAKE<MockMouseDevice>(ScreenPosition{100, 50});
    sut.mouseDevice = mouseDevice;
    
    auto dragHandler = MAKE<TestDragHandler>();
    auto dragModel = MAKE<DragModel>(dragHandler);
    sut.StartDrag(dragModel);
    
    EXPECT_TRUE(sut.IsDragging());
    
    // Calls OnDragUpdate
    sut.OnUpdate({0});
    
    EXPECT_EQ(Vector3(0, 0, 0), dragHandler->lastPosition);
}

