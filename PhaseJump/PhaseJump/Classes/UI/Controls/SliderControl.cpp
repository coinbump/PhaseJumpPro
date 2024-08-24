#include "SliderControl.h"
#include "Log.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

SliderControl::SliderControl() {}

std::optional<float> SliderControl::IntrinsicHeight() {
    auto thumb = Thumb();
    if (nullptr != thumb) {
        return RendererSize(*thumb).y;
    }

    // FUTURE: support more complex layouts, like a thumb that is below the
    // track
    return Base::IntrinsicHeight();
}

PublishedValue<float>& SliderControl::Value() {
    return value;
}

SP<WorldNode> SliderControl::Thumb() {
    GUARDR(owner, nullptr)

    if (!thumb.expired()) {
        return LOCK(thumb);
    }

    auto const& childNodes = owner->ChildNodes();
    GUARDR(!IsEmpty(childNodes), nullptr)

    // Return the first child as the slider thumb
    thumb = *childNodes.begin();
    return LOCK(thumb);
}

void SliderControl::Awake() {
    Base::Awake();

    GUARD(owner)

    valueSubscription =
        value.subject.Receive([this](float value) { this->UpdateThumbPositionForValue(value); });

    auto thumb = Thumb();
    if (nullptr == thumb) {
        PJLog("ERROR. Missing Thumb.");
        return;
    }

    thumbDragHandler = thumb->AddComponent<ThumbDragHandler>();
    thumbDragHandler->target = SCAST<SliderControl>(shared_from_this());
}

// FUTURE: rethink this when we support View sizing for sprites
// std::optional<Vector2> SliderControl::SpriteRendererSize(SpriteRenderer
// spriteRenderer)
//{
//    auto result = Base::SpriteRendererSize(spriteRenderer);
//    if (nullptr != result)
//    {
//        return std::make_optional(new(result.value.x, trackSize));
//    }
//
//    return result;
//}

void SliderControl::UpdateThumbPositionForValue(float value) {
    // Don't move the thumb while the user is dragging it.
    if (nullptr != thumbDragHandler && thumbDragHandler->IsDragging()) {
        return;
    }

    auto thumb = Thumb();
    if (nullptr == thumb) {
        return;
    }
    auto minThumbX = MinThumbX(*thumb);
    auto maxThumbX = MaxThumbX(*thumb);

    value = clamp(value, minValue, maxValue);
    float position = (value - minValue) / (maxValue - minValue);

    auto localPosition = thumb->transform.LocalPosition();
    thumb->transform.SetLocalPosition(
        Vector3(minThumbX + position * (maxThumbX - minThumbX), localPosition.y, localPosition.z)
    );
}

Vector2 SliderControl::TrackSize() {
    GUARDR(owner, Vector2::zero)

    return RendererSize(*owner);
}

Vector2 SliderControl::RendererSize(WorldNode& target) {
    GUARDR(owner, Vector2::zero)

    auto renderer = target.GetComponent<SomeRenderer>();
    GUARDR(renderer, Vector2::zero)

    auto size = renderer->WorldSize();
    GUARDR(size, Vector2::zero)

    return size.value();
}

void SliderControl::OnDragThumbStart(WorldNode& thumb, WorldPosition inputPosition) {
    dragStartInputPosition = inputPosition;
    thumbStartLocalPosition = thumb.transform.LocalPosition();
}

void SliderControl::OnDragThumbEnd() {
    broadcaster.Broadcast(MAKE<EndDragThumbEvent>());
}

float SliderControl::HalfTrackWidth(WorldNode& thumb) {
    auto trackWidth = TrackSize().x;
    auto thumbWidth = RendererSize(thumb).x;

    auto halfTrackWidth = trackWidth / 2.0f - endCapSize - thumbWidth / 2.0f;
    return halfTrackWidth;
}

void SliderControl::OnDragThumbUpdate(WorldNode& thumb, WorldPosition inputPosition) {
    auto x = thumbStartLocalPosition.x + (inputPosition.x - dragStartInputPosition.x);
    auto minThumbX = MinThumbX(thumb);
    auto maxThumbX = MaxThumbX(thumb);
    x = clamp(x, minThumbX, maxThumbX);

    auto newLocalPosition =
        Vector3(x, thumb.transform.LocalPosition().y, thumb.transform.LocalPosition().z);
    thumb.transform.SetLocalPosition(newLocalPosition);

    auto newValue = (newLocalPosition.x - minThumbX) / (maxThumbX - minThumbX);
    Value().SetValue(newValue);
}

// MARK: - ThumbDragHandler

void SliderControl::ThumbDragHandler::OnDragStart(WorldPosition inputPosition) {
    Base::OnDragStart(inputPosition);

    GUARD(owner)
    auto owner = this->owner;
    LOCK(target)->OnDragThumbStart(*owner, inputPosition);
}

void SliderControl::ThumbDragHandler::OnDragEnd() {
    Base::OnDragEnd();

    LOCK(target)->OnDragThumbEnd();
}

void SliderControl::ThumbDragHandler::OnDragUpdate(WorldPosition inputPosition) {
    GUARD(owner)
    auto owner = this->owner;
    LOCK(target)->OnDragThumbUpdate(*owner, inputPosition);
}
