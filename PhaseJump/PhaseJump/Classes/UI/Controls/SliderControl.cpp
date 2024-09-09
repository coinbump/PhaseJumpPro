#include "SliderControl.h"
#include "Log.h"
#include "SomeDragGestureHandler2D.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

SliderControl::SliderControl(Axis2D axis) :
    axis(axis) {}

std::optional<float> SliderControl::IntrinsicHeight() {
    // TODO: come back to this when we work on views
    return std::nullopt;
    //    auto thumbBounds = ThumbBounds();
    //    auto trackBounds = TrackBounds();
    //    auto boundingBounds = thumbBounds + trackBounds;
    //
    //    return boundingBounds.height;
}

Rect SliderControl::TrackFrame() const {
    // TODO: this isn't quite right. Come back to this when we work on views
    //    auto trackSize = TrackSize();
    //    Rect result(Vector2(0, 0), trackSize);
    //    return result;

    Rect result(vec2Zero, vec2Zero);
    return result;
}

std::optional<Rect> SliderControl::ThumbFrame() const {
    // TODO: this isn't quite right. Come back to this when we work on views
    Rect result(vec2Zero, vec2Zero);
    return result;

    //    auto thumbSize = ThumbSize();
    //    Rect result(Vector2(0, thumb->transform.LocalPosition().y), thumbSize);
    //    return result;

    //    auto thumb = Thumb();
    //    GUARDR(thumb, std::nullopt);
    //
    //    auto thumbSize = ThumbSize();
    //    Rect result(Vector2(0, thumb->transform.LocalPosition().y), thumbSize);
    //    return result;
}

PublishedValue<float>& SliderControl::Value() {
    return value;
}

SP<WorldNode> SliderControl::Thumb() const {
    if (!thumb.expired()) {
        return LOCK(thumb);
    }

    return nullptr;
}

void SliderControl::Awake() {
    Base::Awake();

    if (thumb.expired()) {
        auto const& childNodes = owner->ChildNodes();
        if (!IsEmpty(childNodes)) {
            thumb = *childNodes.begin();
        } else {
            PJLog("ERROR. SliderControl is missing thumb.");
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

void SliderControl::UpdateThumbPositionForValue(float value) {
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

Vector2 SliderControl::TrackSize() const {
    GUARDR(owner, {})

    auto rendererSize = RendererSize(*owner);
    Vector2 result;
    result.AxisValue(axis) = frame.size.AxisValue(axis);
    result.AxisValueReverse(axis) = rendererSize.AxisValueReverse(axis);

    return result;
}

Vector2 SliderControl::RendererSize(WorldNode& target) const {
    auto renderer = target.GetComponent<SomeRenderer>();
    GUARDR(renderer, {})

    auto size = renderer->WorldSize();
    GUARDR(size, {})

    return *size;
}

void SliderControl::OnDragThumbStart(WorldNode& thumb, Vector2 inputPosition) {
    dragStartInputPosition = inputPosition;
    thumbStartLocalPosition = thumb.transform.LocalPosition();
}

void SliderControl::OnDragThumbEnd() {}

Vector2 SliderControl::ThumbSize() const {
    auto thumb = Thumb();
    GUARDR(thumb, {})
    return RendererSize(*thumb);
}

float SliderControl::HalfTrackLength(WorldNode& thumb) {
    auto trackWidth = TrackSize().AxisValue(axis);
    auto thumbWidth = ThumbSize().AxisValue(axis);

    auto halfTrackWidth = trackWidth / 2.0f - endCapSize - thumbWidth / 2.0f;
    return halfTrackWidth;
}

void SliderControl::OnDragThumbUpdate(WorldNode& thumb, Vector2 inputPosition) {
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
