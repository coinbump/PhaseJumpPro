#pragma once

#include "Updatable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/14/24
 */
namespace PJ {
    /// A playable timeline-type object
    class Playable : public Updatable {
    public:
        virtual ~Playable() {}

        /// Total playable duration
        virtual float Duration() const = 0;

        /// Normalized progress (0-1.0)
        virtual float Progress() const = 0;

        /// Play from current play time
        virtual void Play() = 0;

        /// Pause. Play resumes at same position
        virtual void Pause() = 0;

        /// Stop. Play resumes at first position
        virtual void Stop() = 0;

        virtual bool IsPlaying() const = 0;

        virtual float PlayTime() const = 0;
        virtual void SetPlayTime(float time) = 0;
    };

    enum class PlayState { Pause, Play, Stop };

    /// Allows us to handle play states with composition
    class SomePlayStateController {
    public:
        virtual ~SomePlayStateController() {}

        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual bool IsPlaying() const = 0;
    };

    /// A simple 3-state play controller for playable logic
    class SimplePlayStateController : public SomePlayStateController {
    public:
        PlayState playState;

        SimplePlayStateController(PlayState playState = PlayState::Pause) :
            playState(playState) {}

        virtual ~SimplePlayStateController() {}

        void Play() {
            playState = PlayState::Play;
        }

        void Pause() {
            playState = PlayState::Pause;
        }

        void Stop() {
            playState = PlayState::Stop;
        }

        bool IsPlaying() const {
            return playState == PlayState::Play;
        }
    };
} // namespace PJ
