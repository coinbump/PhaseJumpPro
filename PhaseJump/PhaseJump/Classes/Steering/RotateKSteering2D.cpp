#include "RotateKSteering2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void RotateKSteering2D::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARD(owner)

    Angle rotation = owner->transform.Rotation2D();
    rotation += Angle::DegreesAngle(turnSpeed.Degrees() * time.delta);
    owner->transform.SetRotation2D(rotation.Clipped());
}
