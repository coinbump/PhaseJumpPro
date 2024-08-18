#pragma once

#include "Playable.h"
#include "SomePlayTimeController.h"
#include "TimeSlice.h"

/*
 RATING: 5 stars
 Has unit tests (via RateFramePlayable)
 CODE REVIEW: 6/14/24
 */
namespace PJ {
    // FUTURE: support PositionFrameAnimator if needed (for complex frame
    // timelines)
    /// Animates a frame value (for frame animations)
    class SomeFramePlayable : public Playable {
    public:
        SimplePlayStateController playStateController{ PlayState::Play };
        UP<SomePlayTimeController> playTimeController;

        virtual ~SomeFramePlayable() {}

        virtual int Frame() const = 0;

        int FrameForUpdate(TimeSlice time) {
            OnUpdate(time);
            return Frame();
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(playTimeController)
            GUARD(playStateController.IsPlaying())

            playTimeController->OnUpdate(time);
        }

        // MARK: Playable

        bool IsPlaying() const override {
            return playStateController.IsPlaying();
        }

        void Play() override {
            playStateController.Play();
        }

        void Pause() override {
            playStateController.Pause();
        }

        void Stop() override {
            playStateController.Stop();
        }
    };
} // namespace PJ
