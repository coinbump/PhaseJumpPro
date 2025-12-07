#include "RotateKSteering2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

FinishType RotateKSteering2D::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARDR(owner, FinishType::Continue)

    Angle rotation = owner->transform.Rotation2D();
    rotation += Angle::WithDegrees(turnSpeed.Degrees() * time.delta);
    owner->transform.SetRotation(rotation.Clipped());

    return FinishType::Continue;
}
