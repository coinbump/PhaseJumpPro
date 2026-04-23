#pragma once

#include "FilePath.h"
#include "Movie.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Mac movie playback via AVFoundation
    /// Objective-C  state is hidden using PIMPL
    class MacMovie : public Movie {
    public:
        using Base = Movie;
        using This = MacMovie;

        struct Config {
            FilePath filePath;
        };

        MacMovie(Config const& config);
        ~MacMovie() override;

        // MARK: Movie

        void Play() override;
        void Pause() override;
        SP<Texture> MakeTexture() override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MacMovie";
        }

    private:
        struct Impl;
        std::unique_ptr<Impl> impl;

        /// Cached texture for the last decoded frame.
        SP<Texture> texture;
    };
} // namespace PJ
