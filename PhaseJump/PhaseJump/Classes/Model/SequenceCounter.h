#pragma once

#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Counts N times up to max count before starting a new sequence
    class SequenceCounter {
    public:
        using This = SequenceCounter;
        using OnFireFunc = std::function<void(This&)>;
        using OnFinishSequenceFunc = std::function<void(This&)>;

    protected:
        int count{};

        /// # of sequence we're on
        int sequenceCount{};

        /// Called to increment the count
        OnFireFunc onFireFunc;

        void OnFinishSequence();

    public:
        /// Max value to count to
        int maxCount{};

        /// Called when the sequence is finished
        OnFinishSequenceFunc onFinishSequenceFunc;

        SequenceCounter(int maxCount);

        void Fire();

        int Count() const {
            return count;
        }

        int SequenceCount() const {
            return sequenceCount;
        }
    };
} // namespace PJ
