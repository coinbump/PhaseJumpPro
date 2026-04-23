#pragma once

#include "Texture.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Abstract video-playback component. Platform implementations (e.g. MacMovie on Mac)
    /// provide Play/Pause and expose the current frame via MakeTexture().
    class Movie : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = Movie;

        ~Movie() override {}

        /// Starts playback
        virtual void Play() = 0;

        /// Pauses playback
        virtual void Pause() = 0;

        /// @return A texture for the current frame, or nullptr if no frame is available yet
        virtual SP<Texture> MakeTexture() = 0;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Movie";
        }
    };
} // namespace PJ
