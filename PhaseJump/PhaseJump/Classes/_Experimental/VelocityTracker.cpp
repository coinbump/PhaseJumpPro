#include "VelocityTracker.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

VelocityTracker::VelocityTracker(PositionFunc _positionFunc) :
    positionFunc(_positionFunc),
    sampler(0.3f, [=]() { return _positionFunc(); }) {
    updatable.onUpdateFunc = [this](auto& updatable, auto time) {
        sampler.OnUpdate(time);
        return FinishType::Continue;
    };
}

Vector3 VelocityTracker::Velocity() const {
    auto& samples = sampler.Samples();
    GUARDR(samples.size() > 5, {});

    Vector3 result;

    // this isn't working. Come back to this
    std::optional<Vector3> prev;
    for (auto& sample : samples) {
        auto value = sample.value;
        if (prev) {
            result += (value - *prev) * sample.delta;
        }
        prev = value;
    }

    result /= samples.size() - 1;
    result = Vector3(1, 1, 1) / result;

    return result;
}
