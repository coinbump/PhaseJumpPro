#pragma once

#include "Axis.h"
#include "ObservedValue.h"
#include "SomeDragHandler.h"
#include "UIControl2D.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests + intrinsic height logic for view layout
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class SomeDragGestureHandler2D;

    /**
     Slider UI control
     Built with 2 nodes and two renderers
     The parent node is the track
     The track node contains the track renderer and a child node which is the thumb
     The thumb node contains a renderer for the thumb

     The thumb node must have a collider attached for hit testing
     */
    class SliderControl : public UIControl2D {
    public:
        // FUTURE: support snap positions
        using Base = UIControl2D;
        using This = SliderControl;

        using OnValueChangeFunc = std::function<void(This&)>;

    protected:
        /// Track node
        WP<WorldNode> track;

        /// Thumb node
        WP<WorldNode> thumb;

        float minValue = 0;
        float maxValue = 1.0f;

        Binding<float> valueBinding;
        OnValueChangeFunc onValueChangeFunc;

        virtual void OnValueChange();

        Axis2D axis = Axis2D::X;
        float trackOrthogonal{};

    public:
        struct Config {
            Axis2D axis = Axis2D::X;

            /// Orthogonal size of the slider track perpendicular to the primary axis
            float trackOrthogonal = 10;

            Binding<float> valueBinding;
        };

        SliderControl(Config config);

        void Configure(Config config);

        void SetThumb(SP<WorldNode> value) {
            thumb = value;
        }

        void SetTrack(SP<WorldNode> value) {
            track = value;
        }

        float Value() const {
            return value;
        }

        void SetOnValueChangeFunc(OnValueChangeFunc value) {
            onValueChangeFunc = value;

            GUARD(onValueChangeFunc)
            onValueChangeFunc(*this);
        }

        void SetValueBinding(Binding<float> binding) {
            valueBinding = binding;
            SetValue(binding);
        }

        void SetValue(float value) {
            this->value.SetValue(value);
        }

        auto& SetEndCapSize(float value) {
            endCapSize = value;
            return *this;
        }

    protected:
        /// Value clamped between minValue and maxValue
        ObservedValue<float> value;

        SP<Cancellable> valueSubscription;

        /// Size of the end caps where the thumb can't reach
        float endCapSize = 0;

        Vector2 dragStartInputPosition;
        Vector3 thumbStartLocalPosition;
        SP<SomeDragGestureHandler2D> thumbDragHandler;

        Vector2 RendererSize(WorldNode& target) const;

        void OnDragThumbStart(WorldNode& thumb, Vector2 inputPosition);
        void OnDragThumbUpdate(WorldNode& thumb, Vector2 inputPosition);
        void OnDragThumbEnd();

        float TrackLength() const;
        Vector2 ThumbSize() const;

        WorldNode* Thumb() const;
        WorldNode* Track() const;

        void UpdateThumbPositionForValue(float value);

        float HalfTrackLength(WorldNode& thumb);

        float MinThumbPos(WorldNode& thumb) {
            return -HalfTrackLength(thumb);
        }

        float MaxThumbPos(WorldNode& thumb) {
            return HalfTrackLength(thumb);
        }

        // MARK: View2D

        void UpdateFrameComponents() override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SliderControl";
        }

        void Awake() override;
    };
} // namespace PJ
