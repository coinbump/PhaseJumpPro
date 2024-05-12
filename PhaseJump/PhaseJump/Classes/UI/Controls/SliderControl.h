#ifndef PJSLIDERCONTROL_H
#define PJSLIDERCONTROL_H

#include "UIControl2D.h"
#include "SomeListener.h"
#include "Broadcaster.h"
#include "PublishedValue.h"
#include "SomeDragHandler.h"
#include "RendererTool.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/11/23
 */
namespace PJ
{
    class SliderControl : public UIControl2D
    {
    protected:
        /// <summary>
        /// Sends thumb drag events to track
        /// </summary>
        class ThumbDragHandler : public SomeDragHandler
        {
        public:
            using Base = SomeDragHandler;

            WP<SliderControl> target;

            void OnDragStart(WorldPosition inputPosition) override;
            void OnDragEnd() override;
            void OnDragUpdate(WorldPosition inputPosition) override;
        };

    public:
        class EndDragThumbEvent : public Event {};

        /// <summary>
        /// The object for the thumb (if nullptr, the first child object is used)
        /// </summary>
        WP<WorldNode> thumb;

        /// <summary>
        /// Size of the end caps where the thumb can't reach
        /// </summary>
        float endCapSize = 0;

        float minValue = 0;
        float maxValue = 1.0f;

        Broadcaster broadcaster;

    protected:
        PublishedValue<float> value{0.0};
        SP<PublishedValue<float>::Subscription> valueSubscription;

        WorldPosition dragStartInputPosition;
        Vector3 thumbStartLocalPosition;
        SP<ThumbDragHandler> thumbDragHandler;
        SP<RendererTool> rendererTool;

    public:
        std::optional<float> IntrinsicHeight() override;
        PublishedValue<float>& Value();
        SP<WorldNode> Thumb();

    protected:
        void Awake() override;

    protected:
        void UpdateThumbPositionForValue(float value);

    public:
        SliderControl();
        Vector2 TrackSize();
        Vector2 RendererSize(WorldNode& target);
        void OnDragThumbStart(WorldNode& thumb, WorldPosition inputPosition);
        void OnDragThumbEnd();

    protected:
        float HalfTrackWidth(WorldNode& thumb);

        float MinThumbX(WorldNode& thumb) { return -HalfTrackWidth(thumb); }
        float MaxThumbX(WorldNode& thumb) { return HalfTrackWidth(thumb); }

        void OnDragThumbUpdate(WorldNode& thumb, WorldPosition inputPosition);
    };
}

#endif
