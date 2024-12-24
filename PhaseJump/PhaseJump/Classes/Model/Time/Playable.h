#pragma once

#include "Macros.h"
#include "PlayStateController.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Objects with a duration, a time state (play time) that can be paused, played, or stopped
    /// In addition, playables are often linked to a controller that determines their play state
    /// Example: Internal simple timer determines the time state for an animation cycle timer.
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

        /// Allows this playable's state and behavior to be defined by another playable object
        /// Used to compose behavior
        Playable* controller{};

    public:
        Playable(PlayState defaultState = PlayState::Play);

        virtual ~Playable() {}

        PlayState GetPlayState() const {
            return playState;
        }

        /// Sets the play state and calls OnPlayStateChange if it changes
        void SetPlayState(PlayState value);

        /// Called when the play state changes
        virtual void OnPlayStateChange();

        /// Sest a func to be called when the play state changes
        void SetOnPlayStateChangeFunc(OnPlayStateChangeFunc value);

        /// Sets a func to be called when the play time changes
        void SetOnPlayTimeChangeFunc(OnPlayTimeChangeFunc value);

        /// @return Returns true if this playable has a controller
        bool HasController() const;

        /// Dispatches a func to the controller if there is one, or if there isn't a controller call
        /// the func
        void Dispatch(std::function<void(Playable&)> func);

        /// Dispatches a func with a result to the controller if there is one, or if there isn't a
        /// controller call the func
        template <class Type>
        Type DispatchResult(std::function<Type(Playable&)> func) const {
            auto controller = Controller();
            GUARDR(controller, Type{})
            return func(*controller);
        }

        /// Resets the playable to its default state
        virtual void Reset();

        /// @return Returns the playable that controls this playable
        /// Used when another object determines the playable state for this playable object
        Playable* Controller() const {
            return controller;
        }

        /// @return Returns total playable duration
        virtual float Duration() const;

        /// Set the total playable duration
        virtual void SetDuration(float value);

        /// @return Returns normalized progress (0-1.0)
        virtual float Progress() const;

        /// Set normalized progress (0-1.0)
        virtual void SetProgress(float value);

        /// Play from current play time
        virtual void Play();

        /// Pause. Play resumes at same position
        virtual void Pause();

        /// Stop. Can't unpause
        virtual void Stop();

        /// @return Returns true if the playable is paused
        virtual bool IsPaused() const;

        /// @return Returns true if the playable is playing
        virtual bool IsPlaying() const;

        /// @return Returns the current time value of the playable
        virtual float PlayTime() const;

        /// Sets the current time value of the playable
        virtual void SetPlayTime(float time);

        /// Called when the play time changes. Subclass is responsible for calling this
        virtual void OnPlayTimeChange();

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;

        bool IsFinished() const override;
    };
} // namespace PJ
