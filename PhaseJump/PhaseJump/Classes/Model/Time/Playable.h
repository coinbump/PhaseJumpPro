#pragma once

#include "Macros.h"
#include "PlayStateController.h"
#include "Updatable.h"

/*
 RATING: 4 stars
 TODO: Needs unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Objects with a duration, a time state (play time) that can be paused, played, or stopped
    /// In addition, playables are often linked to a controller that determines their play state
    /// Example: Internal simple timer determines the time state for an animation cycle timer
    class Playable : public Updatable {
    public:
        using Base = Updatable;
        using This = Playable;

        using OnPlayTimeChangeFunc = std::function<void(This&)>;
        using OnPlayStateChangeFunc = std::function<void(This&)>;

    protected:
        /// Called when the play state changes
        OnPlayStateChangeFunc onPlayStateChangeFunc;

        /// Called when the play time changes
        /// It is the responsibility of the playable to call this func
        OnPlayTimeChangeFunc onPlayTimeChangeFunc;

        /// Play/stop/pause state of the playable
        PlayState playState = PlayState::Play;

        /// Default state the playable reverts to after reset
        PlayState defaultState = PlayState::Play;

    public:
        Playable(PlayState defaultState = PlayState::Play) :
            playState(defaultState),
            defaultState(defaultState) {}

        virtual ~Playable() {}

        /// Sets the play state and calls OnPlayStateChange if it changes
        void SetPlayState(PlayState value) {
            GUARD(value != playState)
            playState = value;
            OnPlayStateChange();
        }

        /// Called when the play state changes
        virtual void OnPlayStateChange() {
            GUARD(onPlayStateChangeFunc)
            onPlayStateChangeFunc(*this);
        }

        /// Set a func to be called when the play state changes
        void SetOnPlayStateChangeFunc(OnPlayStateChangeFunc value) {
            onPlayStateChangeFunc = value;

            // Synchronize state
            OnPlayStateChange();
        }

        /// Set a func to be called when the play time changes
        void SetOnPlayTimeChangeFunc(OnPlayTimeChangeFunc value) {
            onPlayTimeChangeFunc = value;

            // Synchronize state
            OnPlayTimeChange();
        }

        /// @return Returns true if this playable has a controller
        bool HasController() const {
            return Controller() != nullptr;
        }

        /// Dispatch a func to the controller if there is one, or if there isn't a controller call
        /// the func
        void Dispatch(std::function<void(Playable&)> func) {
            auto controller = Controller();
            GUARD(controller)
            func(*controller);
        }

        /// Dispatch a func with a result to the controller if there is one, or if there isn't a
        /// controller call the func
        template <class Type>
        Type DispatchResult(std::function<Type(Playable&)> func) const {
            auto controller = Controller();
            GUARDR(controller, Type{})
            return func(*controller);
        }

        /// Resets the playable to its default state
        virtual void Reset() {
            if (HasController()) {
                Dispatch([](auto& controller) { controller.Reset(); });
            } else {
                SetPlayTime(0);
                SetIsFinished(false);
                SetPlayState(defaultState);
            }
        }

        /// @return Returns the playable that controls this playable
        /// Used when another object determines the playable state for this playable object
        virtual Playable* Controller() const {
            return nullptr;
        }

        /// @return Returns total playable duration
        virtual float Duration() const {
            return DispatchResult<float>([](Playable& controller) { return controller.Duration(); }
            );
        }

        /// Set the total playable duration
        virtual void SetDuration(float value) {
            Dispatch([=](auto& controller) { controller.SetDuration(value); });
        }

        /// @return Returns normalized progress (0-1.0)
        virtual float Progress() const {
            return DispatchResult<float>([](Playable& controller) { return controller.Progress(); }
            );
        }

        /// Set normalized progress (0-1.0)
        virtual void SetProgress(float value) {
            Dispatch([=](auto& controller) { controller.SetProgress(value); });
        }

        /// Play from current play time
        virtual void Play() {
            if (HasController()) {
                Dispatch([](auto& controller) { controller.Play(); });
            } else {
                SetPlayState(PlayState::Play);
            }
        }

        /// Pause. Play resumes at same position
        virtual void Pause() {
            if (HasController()) {
                Dispatch([](auto& controller) { controller.Pause(); });
            } else {
                SetPlayState(PlayState::Pause);
            }
        }

        /// Stop. Can't unpause
        virtual void Stop() {
            if (HasController()) {
                Dispatch([](auto& controller) { controller.Stop(); });
            } else {
                SetPlayState(PlayState::Stop);
            }
        }

        /// @return Returns true if the playable is paused
        virtual bool IsPaused() const {
            if (HasController()) {
                return DispatchResult<bool>([](Playable& controller) {
                    return controller.IsPaused();
                });
            } else {
                return playState == PlayState::Pause;
            }
        }

        /// @return Returns true if the playable is playing
        virtual bool IsPlaying() const {
            GUARDR(!IsFinished(), false)
            if (HasController()) {
                return DispatchResult<bool>([](Playable& controller) {
                    return controller.IsPlaying();
                });
            } else {
                return playState == PlayState::Play;
            }
        }

        /// @return Returns the current time value of the playable
        virtual float PlayTime() const {
            return DispatchResult<float>([](Playable& controller) { return controller.PlayTime(); }
            );
        }

        /// Sets the current time value of the playable
        virtual void SetPlayTime(float time) {
            Dispatch([=](auto& controller) { controller.SetPlayTime(time); });
        }

        /// Called when the play time changes. Subclass is responsible for calling this
        virtual void OnPlayTimeChange() {
            GUARD(onPlayTimeChangeFunc);
            onPlayTimeChangeFunc(*this);
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            if (HasController()) {
                Dispatch([=](auto& controller) { controller.OnUpdate(time); });
            } else {
                Base::OnUpdate(time);
            }
        }

        bool IsFinished() const override {
            GUARDR(!Base::IsFinished(), true);
            return DispatchResult<bool>([](Playable& controller) { return controller.IsFinished(); }
            );
        }
    };
} // namespace PJ
