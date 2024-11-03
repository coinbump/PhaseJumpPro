#include "SequenceCounter.h"

using namespace std;
using namespace PJ;

void SequenceCounter::OnFinishSequence() {
    if (onFinishSequenceFunc) {
        onFinishSequenceFunc(*this);
    }

    sequenceCount++;
}

SequenceCounter::SequenceCounter(int maxCount) :
    maxCount(maxCount) {
    onFireFunc = [](auto& counter) {
        GUARD(counter.maxCount > 0)

        counter.count++;
        if (counter.count >= counter.maxCount) {
            counter.count = 0;
            counter.OnFinishSequence();
        }
    };
}

void SequenceCounter::Fire() {
    GUARD(onFireFunc)
    onFireFunc(*this);
}
