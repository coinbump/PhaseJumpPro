#pragma once

#include "Updatable.h"
#include <deque>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/3/24
 */
namespace PJ {
    /// Samples values in update and keeps the most recent samples, discarding the old ones
    template <class Type>
    class ValueSampler : public SomeUpdatable {
    public:
        using GetFunc = std::function<Type()>;

        struct Sample {
            Type value{};
            float delta{};

            operator Type() const {
                return value;
            }
        };

    protected:
        float maxSampleDuration{};
        GetFunc getFunc;

        std::deque<Sample> samples;
        float samplesDuration{};

        Updatable updatable;

    public:
        std::deque<Sample> const& Samples() const {
            return samples;
        }

        ValueSampler(float _maxSampleDuration, GetFunc _getFunc) :
            maxSampleDuration(_maxSampleDuration),
            getFunc(_getFunc) {
            updatable.onUpdateFunc = [this](auto& updatable, auto time) {
                if (getFunc) {
                    Sample sample;
                    sample.value = getFunc();
                    sample.delta = time.delta;
                    samples.push_back(sample);
                    samplesDuration += time.delta;

                    while (samples.size() > 0 && samplesDuration > maxSampleDuration) {
                        auto& firstSample = *samples.begin();
                        samplesDuration -= firstSample.delta;
                        samples.erase(samples.begin());
                    }
                }

                return FinishType::Continue;
            };
        }

        // MARK: SomeUpdatable

        FinishType OnUpdate(TimeSlice time) override {
            return updatable.OnUpdate(time);
        }

        bool IsFinished() const override {
            return updatable.IsFinished();
        }
    };
} // namespace PJ
