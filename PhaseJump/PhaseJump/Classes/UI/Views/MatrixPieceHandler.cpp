#include "MatrixPieceHandler.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void MatrixPieceHandler::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);
}

void MatrixPieceHandler::SetAnimator(SP<Updatable> value) {
    GUARD(owner)

    if (animator) {
        owner->updatables.Remove(*animator);
    }
    animator = value;
    owner->updatables.Add(animator);
}
