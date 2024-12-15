#include "DialControl.h"
#include "QuickBuild.h"
#include "SomeDragGestureHandler2D.h"

using namespace std;
using namespace PJ;

DialControl::DialControl(Config config) :
    radius(config.radius),
    fullDragDelta(config.fullDragDelta),
    valueBinding(config.valueBinding) {
    SetFixedSize(radius * 2, radius * 2);
}

void DialControl::Awake() {
    Base::Awake();

    QB(*owner)
        .CircleCollider(radius)
        .With<SomeDragGestureHandler2D>()
        .ModifyLatest<SomeDragGestureHandler2D>([](auto& handler) {
            handler.onDragGestureUpdateFunc = [=](auto update) {
                auto dial = update.handler.owner->template TypeComponent<DialControl>();
                GUARD(dial);

                switch (update.type) {
                case SomeDragGestureHandler2D::Update::Type::Start:
                    dial->dragStartValue = dial->valueBinding.Value();
                    break;
                case SomeDragGestureHandler2D::Update::Type::Update:
                    {
                        float value = dial->dragStartValue;
                        auto delta = update.delta;
                        value += delta.y / dial->fullDragDelta;
                        value = std::clamp(value, 0.0f, 1.0f);
                        dial->valueBinding.SetValue(value);
                        break;
                    }
                case SomeDragGestureHandler2D::Update::Type::End:
                    break;
                }
            };
        });
}
