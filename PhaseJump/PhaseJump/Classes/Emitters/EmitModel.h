#pragma once

#include "Tags.h"
#include "Updatable.h"
#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/13/23
 */
namespace PJ {
    /// Tag names for common emit model properties
    namespace EmitModelTag {
        static auto const StartVelocity = "velocity.start";
        static auto const EndVelocity = "velocity.end";
        static auto const StartColor = "color.start";
        static auto const EndColor = "color.end";
        static auto const Offset = "offset";
        static auto const Delay = "delay";
        static auto const Duration = "duration";
        static auto const Angle2D = "angle";
    } // namespace EmitModelTag

    /**
     Interface for an emission model

     Describes how to emit an object from an emitter. Allows us to use simple or complex models
     so we don't have to fill in every detail for simple emitters
     */
    struct EmitModel {
        /// Emit properties
        Tags tags;

        /// Returns start velocity for spawn
        std::optional<Vector3> StartVelocity() const {
            return tags.Value<Vector3>(EmitModelTag::StartVelocity);
        }

        /// Returns end velocity for spawn
        std::optional<Vector3> EndVelocity() const {
            auto result = tags.Value<Vector3>(EmitModelTag::EndVelocity);
            return result.has_value() ? result : StartVelocity();
        }

        /// Returns offset from emitter position for spawn
        std::optional<Vector3> SpawnOffset() const {
            return tags.Value<Vector3>(EmitModelTag::Offset);
        }

        /// Returns delay before creating spawn from emit model (if > 0)
        std::optional<float> SpawnDelay() const {
            return tags.Value<float>(EmitModelTag::Delay);
        }

        /// Returns life duration of spawn (if > 0)
        std::optional<float> Duration() const {
            return tags.Value<float>(EmitModelTag::Duration);
        }
    };

    /// Stores delay progress for delayed emit
    class DelayedEmitModel : public Updatable {
    public:
        EmitModel model;

        /// Used by emitter to track delay progress
        float delay = 0;

        // MARK: Updatable

        DelayedEmitModel(EmitModel model, float delay) :
            model(model),
            delay(delay) {}

        void OnUpdate(TimeSlice time) override {
            GUARD(delay > 0)

            delay -= time.delta;
            delay = std::max(0.0f, delay);
        }
    };
} // namespace PJ
