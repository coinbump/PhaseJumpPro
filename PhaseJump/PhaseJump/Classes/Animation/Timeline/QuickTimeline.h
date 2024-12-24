#pragma once

#include "Timeline.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    enum class AnimatePositionType {
        /// Animate positions relative to starting position
        Relative,

        /// Animate positions as absolute values
        Absolute
    };

    /// Tool to quickly build a timeline that animates a node
    class QuickTimeline {
    public:
        using This = QuickTimeline;

    protected:
        Timeline& timeline;
        WorldNode& node;

        template <class Type>
        This& BuildTrack(
            String trackId, VectorList<Type> const& keys,
            std::function<void(WorldNode&)> configureBindingFunc
        ) {
            for (size_t i = 0; i < keys.size(); i++) {
                Type key = keys[i];

                float normalPos = (float)i / (float)(keys.size() - 1);
                float time = normalPos * timeline.Duration();
                timeline.AddKeyframe<Type>(trackId, time, key);
            }

            GUARDR(configureBindingFunc, *this)
            auto& node = this->node;
            configureBindingFunc(node);

            return *this;
        }

    public:
        QuickTimeline(Timeline& timeline, WorldNode& node);

        /// Moves the node's axis position based on keyframes
        This&
        MoveAxis(Axis axis, VectorList<float> const& keys, AnimatePositionType positionType = {});

        /// Moves the node's X position based on keyframes
        This& MoveX(VectorList<float> const& keys, AnimatePositionType positionType = {});

        /// Moves the node's Y position based on keyframes
        This& MoveY(VectorList<float> const& keys, AnimatePositionType positionType = {});

        /// Moves the node's Z position based on keyframes
        This& MoveZ(VectorList<float> const& keys, AnimatePositionType positionType = {});

        /// Animates scale axis based on keyframes
        This& ScaleAxis(Axis axis, VectorList<float> const& keys);

        /// Animates X scale based on keyframes
        This& ScaleX(VectorList<float> const& keys);

        /// Animates Y scale based on keyframes
        This& ScaleY(VectorList<float> const& keys);

        /// Animates Z scale based on keyframes
        This& ScaleZ(VectorList<float> const& keys);

        /// Animates transform offset based on keyframes
        This& Offset(VectorList<Vector3> const& keys, KeyedTimeType keyedTimeType);

        /// Adds a shake 2D effect (offsets X and Y in random directions)
        This& Shake2D(int count, float maxDistance);

        /// Adds a wobble scale 2D effect (scales X and Y in opposite directions)
        This& Wobble2D(float baseScale = 1.0f, float scaleDelta = 0.2f);
    };
} // namespace PJ
