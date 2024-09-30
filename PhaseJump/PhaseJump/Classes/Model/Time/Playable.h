#pragma once

#include "Macros.h"
#include "PlayStateController.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Objects with a duration, a time state (play time) that can be paused, played, or stopped
    /// In addition, playables are often linked to a driver that determines their play state
    /// Example: Internal simple timer determines the time state for an animation cycle timer
    class Playable : public Updatable {
    public:
        using Base = Updatable;
        using This = Playable;

        using OnPlayTimeChangeFunc = std::function<void(This&)>;

    protected:
        /// Called when the play time changes
        /// It is the responsibility of the playable to call this func
        OnPlayTimeChangeFunc onPlayTimeChangeFunc;

        PlayStateController stateController;

    public:
        virtual ~Playable() {}

        void SetOnPlayTimeChangeFunc(OnPlayTimeChangeFunc value) {
            onPlayTimeChangeFunc = value;

            // Synchronize state
            OnPlayTimeChange();
        }

        void Drive(std::function<void(Playable&)> func) {
            auto driver = PlayDriver();
            GUARD(driver)
            func(*driver);
        }

        bool HasDriver() const {
            return PlayDriver() != nullptr;
        }

        template <class Type>
        Type DriveResult(std::function<Type(Playable&)> func) const {
            auto driver = PlayDriver();
            GUARDR(driver, Type{})
            return func(*driver);
        }

        FinishType GetFinishType() const {
            return IsFinished() ? FinishType::Finish : FinishType::Continue;
        }

        virtual void Reset() {
            if (HasDriver()) {
                Drive([](auto& driver) { driver.Reset(); });
            } else {
                Stop();
                SetPlayTime(0);
                SetIsFinished(false);
            }
        }

        /// Returns the playable that drives this playable
        /// Used when another object determines the playable state for this playable object
        virtual Playable* PlayDriver() const {
            return nullptr;
        }

        /// Total playable duration
        virtual float Duration() const {
            return DriveResult<float>([](Playable& driver) { return driver.Duration(); });
        }

        /// Set total playable duration
        virtual void SetDuration(float value) {
            Drive([=](auto& driver) { driver.SetDuration(value); });
        }

        /// Normalized progress (0-1.0)
        virtual float Progress() const {
            return DriveResult<float>([](Playable& driver) { return driver.Progress(); });
        }

        /// Set normalized progress (0-1.0)
        virtual void SetProgress(float value) {
            Drive([=](auto& driver) { driver.SetProgress(value); });
        }

        /// Play from current play time
        virtual void Play() {
            if (HasDriver()) {
                Drive([](auto& driver) { driver.Play(); });
            } else {
                stateController.Play();
            }
        }

        /// Pause. Play resumes at same position
        virtual void Pause() {
            if (HasDriver()) {
                Drive([](auto& driver) { driver.Pause(); });
            } else {
                stateController.Pause();
            }
        }

        /// Stop. Play resumes at first position
        virtual void Stop() {
            if (HasDriver()) {
                Drive([](auto& driver) { driver.Stop(); });
            } else {
                stateController.Stop();
            }
        }

        /// @return Returns true if the playable is paused
        virtual bool IsPaused() const {
            if (HasDriver()) {
                return DriveResult<bool>([](Playable& driver) { return driver.IsPaused(); });
            } else {
                return stateController.IsPaused();
            }
        }

        /// @return Returns true if the playable is playing
        virtual bool IsPlaying() const {
            GUARDR(!IsFinished(), false)
            if (HasDriver()) {
                return DriveResult<bool>([](Playable& driver) { return driver.IsPlaying(); });
            } else {
                return stateController.IsPlaying();
            }
        }

        /// @return Returns the current time value of the playable
        virtual float PlayTime() const {
            return DriveResult<float>([](Playable& driver) { return driver.PlayTime(); });
        }

        /// Sets the current time value of the playable
        virtual void SetPlayTime(float time) {
            Drive([=](auto& driver) { driver.SetPlayTime(time); });
        }

        virtual void OnPlayTimeChange() {
            GUARD(onPlayTimeChangeFunc);
            onPlayTimeChangeFunc(*this);
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            Drive([=](auto& driver) { driver.OnUpdate(time); });
        }

        bool IsFinished() const override {
            GUARDR(!Base::IsFinished(), true);
            return DriveResult<bool>([](Playable& driver) { return driver.IsFinished(); });
        }
    };
} // namespace PJ
