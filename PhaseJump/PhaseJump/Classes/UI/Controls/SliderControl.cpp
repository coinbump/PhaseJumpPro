#include "SliderControl.h"
#include "Dev.h"
#include "QuickBuild.h"
#include "SomeDragGestureHandler2D.h"
#include "SomeRenderer.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

using This = SliderControl;

SliderControl::SliderControl(Config config) :
    axis(config.axis),
    minValue(config.minValue),
    maxValue(config.maxValue),
    trackOrthogonal(config.trackOrthogonal) {
    GetUpdatable().onUpdateFunc = [this](auto& updatable, auto time) {
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

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.Text([this]() {
            return UIPlanner::TextConfig{ .label = "Value",
                                          .text = std::format("{}", value.Value()) };
        });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);

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

    QB(*this->owner).RectCollider({ 1, 1 });

    dragHandler = owner->AddComponentPtr<SomeDragGestureHandler2D>();
    dragHandler->onDragGestureUpdateFunc = [this](auto update) {
        switch (update.type) {
        case SomeDragGestureHandler2D::Update::Type::Start:
            OnDragStart(update.worldPosition);
            break;
        case SomeDragGestureHandler2D::Update::Type::End:
            OnDragEnd();
            break;
        case SomeDragGestureHandler2D::Update::Type::Update:
            OnDragUpdate(update.worldPosition);
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
    return frame.size.AxisValue(axis);
}

Vector2 SliderControl::RendererSize(WorldNode& target) const {
    auto renderer = target.GetComponent<SomeRenderer>();
    GUARDR(renderer, {})

    auto size = renderer->WorldSize();
    return size;
}

void SliderControl::OnDragStart(Vector2 inputWorldPos) {
    dragStartInputWorldPos = inputWorldPos;
}

void SliderControl::OnDragEnd() {}

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

void SliderControl::OnDragUpdate(Vector2 inputWorldPos) {
    auto viewPos = WorldToView(inputWorldPos);

    float minAxisValue = endCapSize;
    float maxAxisValue = frame.size.AxisValue(axis) - endCapSize;
    GUARD(maxAxisValue > 0)

    viewPos.AxisValue(axis) = std::clamp(viewPos.AxisValue(axis), minAxisValue, maxAxisValue);

    auto newValueNormal = (viewPos.AxisValue(axis) - minAxisValue) / (maxAxisValue - minAxisValue);

    switch (axis) {
    case Axis2D::Y:
        // Vertical sliders have their lowest value at the bottom
        newValueNormal = 1.0f - newValueNormal;
        break;
    default:
        break;
    }

    auto newValue = minValue + (maxValue - minValue) * newValueNormal;

    SetValue(newValue);
    UpdateThumbPositionForValue(newValue);
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

void SliderControl::SetThumb(SP<WorldNode> value) {
    thumb = value;
}

void SliderControl::SetTrack(SP<WorldNode> value) {
    track = value;
}

void SliderControl::SetOnValueChangeFunc(OnValueChangeFunc value) {
    onValueChangeFunc = value;

    GUARD(onValueChangeFunc)
    onValueChangeFunc(*this);
}

void SliderControl::SetValueBinding(Binding<float> binding) {
    valueBinding = binding;
    SetValue(binding);
}

void SliderControl::SetValue(float value) {
    this->value.SetValue(value);
}

This& SliderControl::SetEndCapSize(float value) {
    endCapSize = value;
    return *this;
}

bool SliderControl::IsDragging() const {
    return dragHandler && dragHandler->IsDragging();
}
