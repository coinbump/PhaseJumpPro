#pragma once

#include "TimeTrack.h"
#include "VectorList.h"

// TODO: finish this
// CODE REVIEW: /23
namespace PJ {
    /// Contains multiple playable tracks
    class Timeline : public Playable {
    public:
        using This = Timeline;
        using Base = Playable;
        using TrackList = VectorList<UP<Playable>>;

        /// Defines default behavior for how this timeline handles its tracks
        /// Can be modified by changing the corresponding funcs
        enum PlayType {
            /// Timeline handles update events and syncs its play time to the tracks
            TimelineDrives,

            /// Tracks handle their own update events
            TracksDrive
        };

    protected:
        AnimationCycleTimer timer;
        Playable* playDriver = &timer;

        TrackList tracks;

    public:
        Timeline(float duration, AnimationCycleType cycleType, PlayType playType) :
            timer(duration, cycleType) {
            timer.SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });

            switch (playType) {
            case PlayType::TimelineDrives:
                {
                    OnPlayTimeChangeFunc overrideFunc = [](auto& playable) {
                        Timeline& timeline = *(static_cast<Timeline*>(&playable));

                        float playTime = timeline.PlayTime();
                        for (auto& track : timeline.tracks) {
                            track->SetPlayTime(playTime);
                        }
                    };
                    Override(onPlayTimeChangeFunc, overrideFunc);
                    break;
                }
            case PlayType::TracksDrive:
                {
                    onUpdateFunc = [](auto& updatable, TimeSlice time) {
                        Timeline& timeline = *(static_cast<Timeline*>(&updatable));

                        bool anyTracksNotFinished = false;
                        for (auto& track : timeline.tracks) {
                            track->OnUpdate(time);

                            if (!track->IsFinished()) {
                                anyTracksNotFinished = true;
                            }
                        }

                        return anyTracksNotFinished ? FinishType::Continue : FinishType::Finish;
                    };
                    break;
                }
            }
        }

        TrackList const& Tracks() const {
            return tracks;
        }

        TrackList::iterator Add(UP<Playable>& track) {
            GUARDR(track, tracks.end())
            return tracks.emplace(tracks.end(), std::move(track));
        }

        // MARK: Playable

        Playable* PlayDriver() const override {
            return playDriver;
        }
    };
} // namespace PJ
