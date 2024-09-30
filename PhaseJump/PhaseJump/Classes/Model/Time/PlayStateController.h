#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    enum class PlayState { Play, Pause, Stop };

    /// 3-state play controller for playable logic
    class PlayStateController {
    public:
        PlayState playState;

        PlayStateController(PlayState playState = PlayState::Play) :
            playState(playState) {}

        virtual ~PlayStateController() {}

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

        bool IsPaused() const {
            return playState == PlayState::Pause;
        }
    };
} // namespace PJ
