#include "ScrollBarControl.h"
#include "Dev.h"
#include "SomeDragGestureHandler2D.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

ScrollBarControl::ScrollBarControl(Axis2D axis) :
    axis(axis) {}

PublishedValue<float>& ScrollBarControl::Value() {
    return value;
}

SP<WorldNode> ScrollBarControl::Thumb() const {
    return LOCK(thumb);
}

void ScrollBarControl::Awake() {
    Base::Awake();

    if (thumb.expired()) {
        auto const& childNodes = owner->ChildNodes();
        if (!IsEmpty(childNodes)) {
            thumb = *childNodes.begin();
        } else {
            PJ::Log("ERROR. ScrollBarControl is missing thumb.");
            return;
        }
    }

    auto thumbLock = LOCK(thumb);
    GUARD(thumbLock)

    WP<This> weakThis = SCAST<This>(shared_from_this());

    thumbDragHandler = thumbLock->AddComponentPtr<SomeDragGestureHandler2D>();
    thumbDragHandler->onDragGestureUpdateFunc = [weakThis](auto update) {
        GUARD(!weakThis.expired())
        auto target = weakThis.lock();

        switch (update.type) {
        case SomeDragGestureHandler2D::Update::Type::Start:
            target->OnDragThumbStart(*update.gesture.owner, update.worldPosition);
            break;
        case SomeDragGestureHandler2D::Update::Type::End:
            target->OnDragThumbEnd();
            break;
        case SomeDragGestureHandler2D::Update::Type::Update:
            target->OnDragThumbUpdate(*update.gesture.owner, update.worldPosition);
            break;
        }
    };

    valueSubscription =
        value.Receive([this](float value) { this->UpdateThumbPositionForValue(value); });
}

void ScrollBarControl::UpdateThumbPositionForValue(float value) {
    // Don't move the thumb while the user is dragging it.
    if (nullptr != thumbDragHandler && thumbDragHandler->IsDragging()) {
        return;
    }

    auto thumb = Thumb();
    GUARD(thumb)

    auto minThumbPos = MinThumbPos(*thumb);
    auto maxThumbPos = MaxThumbPos(*thumb);

    value = clamp(value, minValue, maxValue);
    float position = (value - minValue) / (maxValue - minValue);

    auto localPosition = thumb->transform.LocalPosition();

    Vector2 newPos;
    newPos.AxisValue(axis) = minThumbPos + position * (maxThumbPos - minThumbPos);
    newPos.AxisValueReverse(axis) = ((Vector2)localPosition).AxisValueReverse(axis);

    thumb->transform.SetLocalPosition(Vector3(newPos.x, newPos.y, localPosition.z));
}

Vector2 ScrollBarControl::TrackSize() const {
    GUARDR(owner, {})

    auto rendererSize = RendererSize(*owner);
    Vector2 result;
    result.AxisValue(axis) = frame.size.AxisValue(axis);
    result.AxisValueReverse(axis) = rendererSize.AxisValueReverse(axis);

    return result;
}

Vector2 ScrollBarControl::RendererSize(WorldNode& target) const {
    auto renderer = target.GetComponent<SomeRenderer>();
    GUARDR(renderer, {})

    auto size = renderer->WorldSize();
    GUARDR(size, {})

    return *size;
}

void ScrollBarControl::OnDragThumbStart(WorldNode& thumb, Vector2 inputPosition) {
    dragStartInputPosition = inputPosition;
    thumbStartLocalPosition = thumb.transform.LocalPosition();
}

void ScrollBarControl::OnDragThumbEnd() {}

Vector2 ScrollBarControl::ThumbSize() const {
    auto thumb = Thumb();
    GUARDR(thumb, {})
    return RendererSize(*thumb);
}

float ScrollBarControl::HalfTrackLength(WorldNode& thumb) {
    auto trackWidth = TrackSize().AxisValue(axis);
    auto thumbWidth = ThumbSize().AxisValue(axis);

    auto halfTrackWidth = trackWidth / 2.0f - endCapSize - thumbWidth / 2.0f;
    return halfTrackWidth;
}

void ScrollBarControl::OnDragThumbUpdate(WorldNode& thumb, Vector2 inputPosition) {
    auto pos = ((Vector2)thumbStartLocalPosition).AxisValue(axis) +
               (inputPosition.AxisValue(axis) - dragStartInputPosition.AxisValue(axis));
    auto minThumbPos = MinThumbPos(thumb);
    auto maxThumbPos = MaxThumbPos(thumb);
    pos = clamp(pos, minThumbPos, maxThumbPos);

    auto thumbPosition = thumb.transform.LocalPosition();

    Vector2 newPos;
    newPos.AxisValue(axis) = pos;
    newPos.AxisValueReverse(axis) = ((Vector2)thumbPosition).AxisValueReverse(axis);

    auto newLocalPosition = Vector3(newPos.x, newPos.y, thumbPosition.z);
    thumb.transform.SetLocalPosition(newLocalPosition);

    auto newValue = (newPos.AxisValue(axis) - minThumbPos) / (maxThumbPos - minThumbPos);
    Value().SetValue(newValue);
}
