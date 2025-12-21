#pragma once

#include "Axis.h"
#include "DragHandler.h"
#include "ObservedValue.h"
#include "UIControl2D.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class DragGestureHandler2D;

    /**
     Slider UI control

     Built with optional child nodes for the track and thumb
     */
    class SliderControl : public UIControl2D {
    public:
        // FUTURE: support snap positions
        using Base = UIControl2D;
        using This = SliderControl;

        using OnValueChangeFunc = std::function<void(This&)>;

    protected:
        /// (Optional). Track node
        WP<WorldNode> track;

        /// (Optional). Thumb node
        WP<WorldNode> thumb;

        /// Minimum control value
        float minValue = 0;

        /// Maximum control value
        float maxValue = 1.0f;

        /// Control value binding
        Binding<float> valueBinding;

        /// Called when the control value changes
        OnValueChangeFunc onValueChangeFunc;

        /// Called when the control value changes
        virtual void OnValueChange();

        /// Primary axis for this control
        Axis2D axis = Axis2D::X;

        /// Track size along the orthogonal axis
        float trackOrthogonal{};

    public:
        struct Config {
            Axis2D axis = Axis2D::X;

            /// Minimum control value
            float minValue = 0;

            /// Maximum control value
            float maxValue = 1.0f;

            /// Track size along the orthogonal axis
            float trackOrthogonal = 10;

            Binding<float> valueBinding;
        };

        SliderControl(Config const& config);

        /// @return Returns true if the slider is tracking a drag gesture
        bool IsDragging() const;

        /// Associates a thumb node with this slider
        void SetThumb(SP<WorldNode> value);

        /// Associates a track node with this slider
        void SetTrack(SP<WorldNode> value);

        /// @return Returns the control value
        float Value() const {
            return value;
        }

        /// @return Returns the normalized control value (0-1.0)
        float NormalValue() const {
            return (value - minValue) / (maxValue - minValue);
        }

        /// Sets a func to be called when the control value changes
        void SetOnValueChangeFunc(OnValueChangeFunc value);

        /// Sets a binding for the control value
        void SetValueBinding(Binding<float> binding);

        /// Sets the control value and the bound value if a binding exists
        void SetValue(float value);

        /// Sets the end cap size, that limits how far the thumb can travel
        /// Ignored if the thumb is not a child node of the slider
        This& SetEndCapSize(float value);

    protected:
        /// Value clamped between minValue and maxValue
        ObservedValue<float> value;

        /// Size of the end caps where the thumb can't reach
        float endCapSize = 0;

        Vector2 dragStartInputWorldPos;

        /// Drag gesture handler for slider tracking
        SP<DragGestureHandler2D> dragHandler;

        Vector2 RendererSize(WorldNode& target) const;

        void OnDragStart(Vector2 inputWorldPos);
        void OnDragUpdate(Vector2 inputWorldPos);
        void OnDragEnd();

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
