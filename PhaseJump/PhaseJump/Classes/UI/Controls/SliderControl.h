#pragma once

#include "Axis.h"
#include "PublishedValue.h"
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

        /// The object for the thumb (if not specified, the first child object is
        /// used)
        WP<WorldNode> thumb;

        float minValue = 0;
        float maxValue = 1.0f;

    public:
        Axis2D axis = Axis2D::X;

        SliderControl(Axis2D axis = Axis2D::X);

        PublishedValue<float>& Value();

        auto& SetEndCapSize(float value) {
            endCapSize = value;
            return *this;
        }

        // MARK: View2D

        std::optional<float> IntrinsicHeight() override;

    protected:
        /// Value clamped between minValue and maxValue
        PublishedValue<float> value{ 0.0 };

        SP<Cancellable> valueSubscription;

        /// Size of the end caps where the thumb can't reach
        float endCapSize = 0;

        Vector2 dragStartInputPosition;
        Vector3 thumbStartLocalPosition;
        SP<SomeDragGestureHandler2D> thumbDragHandler;

        Rect TrackFrame() const;
        std::optional<Rect> ThumbFrame() const;
        Vector2 RendererSize(WorldNode& target) const;

        void OnDragThumbStart(WorldNode& thumb, Vector2 inputPosition);
        void OnDragThumbUpdate(WorldNode& thumb, Vector2 inputPosition);
        void OnDragThumbEnd();

        Vector2 TrackSize() const;
        Vector2 ThumbSize() const;

        SP<WorldNode> Thumb() const;

        void UpdateThumbPositionForValue(float value);

        float HalfTrackLength(WorldNode& thumb);

        float MinThumbPos(WorldNode& thumb) {
            return -HalfTrackLength(thumb);
        }

        float MaxThumbPos(WorldNode& thumb) {
            return HalfTrackLength(thumb);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SliderControl";
        }

        void Awake() override;
    };
} // namespace PJ
