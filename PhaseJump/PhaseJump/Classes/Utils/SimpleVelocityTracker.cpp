#include "SimpleVelocityTracker.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

SimpleVelocityTracker::SimpleVelocityTracker(PositionFunc _positionFunc) :
    positionFunc(_positionFunc) {
    onUpdateFunc = [this](auto& updatable, auto time) {
        prevPosition = position;
        position = positionFunc();
        timeDelta = time.delta;

        return FinishType::Continue;
    };
}

Vector3 SimpleVelocityTracker::Velocity() const {
    // TODO: problem: character doesn't necessarily move every frame, so this will often be zero
    GUARDR(prevPosition && timeDelta > 0, {})
    Vector3 distance = position - *prevPosition;
    Vector3 velocity = distance / timeDelta;
    return velocity;
}
