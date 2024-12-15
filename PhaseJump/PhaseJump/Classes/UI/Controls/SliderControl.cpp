#include "SliderControl.h"
#include "Dev.h"
#include "SomeDragGestureHandler2D.h"
#include "SomeRenderer.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

SliderControl::SliderControl(Config config) :
    axis(config.axis),
    trackOrthogonal(config.trackOrthogonal) {
    updatable.onUpdateFunc = [this](auto& updatable, auto time) {
        if (valueBinding.getFunc) {
            SetValue(valueBinding.getFunc());
        }
        return FinishType::Continue;
    };

    IdealSizeFunc idealSizeFunc = [this](auto& view, auto proposal) {
        auto thumbSize = ThumbSize();

        switch (this->axis) {
        case Axis2D::X:
            {
                auto maxOrthogonal = std::max(thumbSize.y, trackOrthogonal);
                return ViewSizeProposal{ .width = FloatMath::maxValue, .height = maxOrthogonal };
            }
        case Axis2D::Y:
            {
                auto maxOrthogonal = std::max(thumbSize.x, trackOrthogonal);
                return ViewSizeProposal{ .width = maxOrthogonal, .height = FloatMath::maxValue };
            }
        }
    };

    switch (this->axis) {
    case Axis2D::X:
        SetIsIdealSize(false, true);
        break;
    case Axis2D::Y:
        SetIsIdealSize(true, false);
        break;
    }
    SetIdealSizeFunc(idealSizeFunc);

    value.SetOnValueChangeFunc([this](auto value) { OnValueChange(); });

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        planner.Text("Value", [this]() {
            stringstream ss;
            ss << MakeString(value);
            return ss.str();
        });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);

    SetValueBinding(config.valueBinding);
}

void SliderControl::Configure(Config config) {
    axis = config.axis;
    trackOrthogonal = config.trackOrthogonal;
    SetValueBinding(config.valueBinding);
}

WorldNode* SliderControl::Thumb() const {
    GUARDR(!thumb.expired(), {})
    return thumb.lock().get();
}

WorldNode* SliderControl::Track() const {
    GUARDR(!track.expired(), {})
    return track.lock().get();
}

void SliderControl::Awake() {
    Base::Awake();

    GUARD_LOG(!track.expired(), "ERROR. SliderControl is missing track.");
    GUARD_LOG(!thumb.expired(), "ERROR. SliderControl is missing thumb.");

    auto thumbLock = LOCK(thumb);
    GUARD(thumbLock)

    WP<This> weakThis = SCAST<This>(shared_from_this());

    thumbDragHandler = thumbLock->AddComponentPtr<SomeDragGestureHandler2D>();
    thumbDragHandler->onDragGestureUpdateFunc = [weakThis](auto update) {
        GUARD(!weakThis.expired())
        auto target = weakThis.lock();

        switch (update.type) {
        case SomeDragGestureHandler2D::Update::Type::Start:
            target->OnDragThumbStart(*update.handler.owner, update.worldPosition);
            break;
        case SomeDragGestureHandler2D::Update::Type::End:
            target->OnDragThumbEnd();
            break;
        case SomeDragGestureHandler2D::Update::Type::Update:
            target->OnDragThumbUpdate(*update.handler.owner, update.worldPosition);
            break;
        }
    };

    OnValueChange();
}

void SliderControl::OnValueChange() {
    float value = this->value;
    UpdateThumbPositionForValue(value);

    valueBinding.SetValue(value);

    GUARD(onValueChangeFunc)
    onValueChangeFunc(*this);
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

    SetValue(clamp(value, minValue, maxValue));
    float position = (value - minValue) / (maxValue - minValue);

    auto localPosition = thumb->transform.LocalPosition();

    Vector2 newPos;
    newPos.AxisValue(axis) = minThumbPos + position * (maxThumbPos - minThumbPos);
    newPos.AxisValueOrthogonal(axis) = ((Vector2)localPosition).AxisValueOrthogonal(axis);

    thumb->transform.SetLocalPosition(Vector3(newPos.x, newPos.y, localPosition.z));
}

float SliderControl::TrackLength() const {
    auto track = Track();
    GUARDR(track, {})
    return RendererSize(*track).AxisValue(axis);
}

Vector2 SliderControl::RendererSize(WorldNode& target) const {
    auto renderer = target.GetComponent<SomeRenderer>();
    GUARDR(renderer, {})

    auto size = renderer->WorldSize();
    return size;
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
    auto trackWidth = TrackLength();
    auto thumbWidth = ThumbSize().AxisValue(axis);

    auto halfTrackWidth = trackWidth / 2.0f - endCapSize - thumbWidth / 2.0f;
    return halfTrackWidth;
}

void SliderControl::OnDragThumbUpdate(WorldNode& thumb, Vector2 inputPosition) {
    auto pos = ((Vector2)thumbStartLocalPosition).AxisValue(axis) +
               (inputPosition.AxisValue(axis) - dragStartInputPosition.AxisValue(axis));
    auto minThumbPos = MinThumbPos(thumb);
    auto maxThumbPos = MaxThumbPos(thumb);
    GUARD(maxThumbPos > minThumbPos)

    pos = clamp(pos, minThumbPos, maxThumbPos);

    auto thumbPosition = thumb.transform.LocalPosition();

    Vector2 newPos;
    newPos.AxisValue(axis) = pos;
    newPos.AxisValueOrthogonal(axis) = ((Vector2)thumbPosition).AxisValueOrthogonal(axis);

    auto newLocalPosition = Vector3(newPos.x, newPos.y, thumbPosition.z);
    thumb.transform.SetLocalPosition(newLocalPosition);

    auto newValue = (newPos.AxisValue(axis) - minThumbPos) / (maxThumbPos - minThumbPos);
    SetValue(newValue);
}

void SliderControl::UpdateFrameComponents() {
    Base::UpdateFrameComponents();

    auto track = Track();
    if (track) {
        auto trackRenderer = track->TypeComponent<SomeRenderer>();
        if (trackRenderer) {
            Vector2 trackSize = frame.size;
            trackSize.AxisValueOrthogonal(axis) = trackOrthogonal;
            trackRenderer->SetWorldSize({ trackSize.x, trackSize.y, 0 });
        }
    }

    UpdateThumbPositionForValue(value);
}
