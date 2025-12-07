#include "DirectionKSteering2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

DirectionKSteering2D::DirectionKSteering2D(float velocity, float acceleration, float maxVelocity) :
    velocity(velocity),
    acceleration(acceleration),
    maxVelocity(maxVelocity) {}

FinishType DirectionKSteering2D::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARDR(owner, FinishType::Continue)

    velocity += acceleration * time.delta;
    if (maxVelocity > 0) {
        velocity = std::min(maxVelocity, velocity);
    }

    Vector2 moveVector = owner->transform.Rotation2D().ToVector2();
    float moveDeltaValue = velocity * time.delta;
    Vector2 moveDelta = moveVector * moveDeltaValue;
    owner->transform.SetLocalPosition(owner->transform.LocalPosition() + moveDelta);

    return FinishType::Continue;
}
