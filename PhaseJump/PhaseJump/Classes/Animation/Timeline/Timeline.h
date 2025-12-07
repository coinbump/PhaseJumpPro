#pragma once

#include "DiagnoseModel.h"
#include "TimeTrack.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/14/24
 */
namespace PJ {
    template <class Type>
    struct TimelineKeyframeConfig {
        ValueKeyframeConfig<Type> keyframeConfig;

        /// Use to set the track config when adding a new track by adding a keyframe
        /// Ignored after the track is creataed
        KeyedTimeType timeType = KeyedTimeType::Interpolate;
    };

    /// Contains multiple playable tracks
    class Timeline : public Playable {
    public:
        using This = Timeline;
        using Base = Playable;
        using TrackList = VectorList<UP<SomeTimeTrack>>;

    protected:
        AnimationCycleTimer timer;

        TrackList tracks;

        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

    public:
        struct Config {
            float duration{};
            AnimationCycleType cycleType{};
        };

        Timeline(Config const& config);

        TrackList const& Tracks() const {
            return tracks;
        }

        TrackList::iterator Add(UP<SomeTimeTrack>& track);
        TrackList::iterator FindTrackIterator(String id);
        TrackList::const_iterator FindTrackIterator(String id) const;
        SomeTimeTrack* FindTrack(String id) const;

        /// Set the the binding for the track, based on id
        /// Each track needs a binding func in order to change values when the track value changes
        template <class Type>
        void SetTrackBinding(String id, SetBindingFunc<Type> setBindingFunc) {
            auto existingTrackI = FindTrackIterator(id);
            GUARD_LOG(
                existingTrackI != tracks.end(), "ERROR: Configure track before setting binding."
            )

            auto track = dynamic_cast<TimeTrack<Type>*>(existingTrackI->get());
            GUARD_LOG(track, "ERROR. Conflicting types for track: " + id);

            track->setBindingFunc = setBindingFunc;
            GUARD(setBindingFunc)

            // Synchronize states
            setBindingFunc(track->Value());
        }

        /// Add a keyframe to the track with id
        template <class Type>
        ValueKeyframe<Type>*
        AddKeyframe(String id, float time, TimelineKeyframeConfig<Type> const& config) {
            using Track = TimeTrack<Type>;
            Track* track{};

            auto existingTrackI = FindTrackIterator(id);
            if (existingTrackI != tracks.end()) {
                track = dynamic_cast<Track*>(existingTrackI->get());
                GUARDR_LOG(track, {}, "ERROR. Conflicting types for track: " + id);
            } else {
                typename Track::Config trackConfig{ .id = id,
                                                    .duration = timer.Duration(),
                                                    .cycleType = AnimationCycleType::Once,
                                                    .keyedTimeType = config.timeType };
                UP<SomeTimeTrack> trackToAdd = NEW<Track>(trackConfig);
                track = static_cast<Track*>(Add(trackToAdd)->get());
            }

            UP<ValueKeyframe<Type>> kf = NEW<Keyframe<Type>>(config.keyframeConfig);
            auto result = static_cast<ValueKeyframe<Type>*>(kf.get());
            track->Add(kf, time);

            return result;
        }

        /// Add a keyframe to the track with id
        template <class Type>
        ValueKeyframe<Type>* AddKeyframe(String id, float time, Type value) {
            ValueKeyframeConfig<Type> keyframeConfig;
            keyframeConfig.value = value;

            TimelineKeyframeConfig<Type> config{ .keyframeConfig = keyframeConfig };
            return AddKeyframe(id, time, config);
        }

        // MARK: Updatable

        FinishType OnUpdate(TimeSlice time) override {
            PJ::LogIf(_diagnose, "Timeline time: ", MakeString(PlayTime()));

            return Base::OnUpdate(time);
        }
    };
} // namespace PJ
