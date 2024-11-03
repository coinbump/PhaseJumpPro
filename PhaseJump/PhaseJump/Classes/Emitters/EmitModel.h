#pragma once

#include "Tags.h"
#include "Updatable.h"
#include "Vector2.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/13/24
 */
namespace PJ {
    /// Tag names for common emit model properties
    namespace EmitModelTag {
        auto constexpr StartVelocity = "velocity.start";
        auto constexpr EndVelocity = "velocity.end";
        auto constexpr StartDirectionVelocity = "velocity.dir.start";
        auto constexpr EndDirectionVelocity = "velocity.dir.end";
        auto constexpr StartColor = "color.start";
        auto constexpr EndColor = "color.end";
        auto constexpr Offset = "offset";
        auto constexpr Delay = "delay";
        auto constexpr Duration = "duration";
        auto constexpr Angle = "angle";
    } // namespace EmitModelTag

    /**
     Interface for an emission model

     Describes how to emit an object from an emitter. Allows us to use simple or complex models
     so we don't have to fill in every detail for simple emitters
     */
    struct EmitModel {
        /// Emit properties
        Tags tags;

        /// @return Returns start velocity for spawn
        std::optional<Vector3> StartVelocity() const {
            return tags.Value<Vector3>(EmitModelTag::StartVelocity);
        }

        /// @return Returns end velocity for spawn
        std::optional<Vector3> EndVelocity() const {
            auto result = tags.Value<Vector3>(EmitModelTag::EndVelocity);
            return result.has_value() ? result : StartVelocity();
        }

        /// @return Returns start directional velocity for spawn
        std::optional<float> StartDirectionVelocity() const {
            return tags.Value<float>(EmitModelTag::StartDirectionVelocity);
        }

        /// @return Returns end directional velocity for spawn
        std::optional<float> EndDirectionVelocity() const {
            auto result = tags.Value<float>(EmitModelTag::EndDirectionVelocity);
            return result.has_value() ? result : StartDirectionVelocity();
        }

        /// @return Returns offset from emitter position for spawn
        std::optional<Vector3> Offset() const {
            return tags.Value<Vector3>(EmitModelTag::Offset);
        }

        /// @return Returns delay before creating spawn from emit model (if > 0)
        std::optional<float> Delay() const {
            return tags.Value<float>(EmitModelTag::Delay);
        }

        /// @return Returns life duration of spawn (if > 0)
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

        DelayedEmitModel(EmitModel model, float delay) :
            model(model),
            delay(delay) {}

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(delay > 0)

            delay -= time.delta;
            delay = std::max(0.0f, delay);
        }
    };
} // namespace PJ
