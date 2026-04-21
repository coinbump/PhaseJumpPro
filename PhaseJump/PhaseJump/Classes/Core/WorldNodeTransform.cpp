#include "WorldNodeTransform.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

Vector3 WorldNodeTransform::WorldPosition() const {
    auto result = geo.position;

    auto world = owner.World();
    GUARDR(world, result)

    GUARDR(owner.Parent(), result)

    // Use ParentWorldModelMatrix (not WorldModelMatrix(parent)) so the frame matches how
    // descendants of a Viewport compute their positions — same viewport translate-only rule
    // applies to the parent itself, keeping direct children and grandchildren consistent.
    auto worldModelMatrix = world->ParentWorldModelMatrix(owner);
    result = worldModelMatrix.MultiplyPoint(result);

    return result;
}

void WorldNodeTransform::SetWorldPosition(Vector3 position, SetLocalPosFunc func) {
    auto world = owner.World();
    auto parentNode = owner.Parent();

    if (world && parentNode) {
        auto worldModelMatrix = world->ParentWorldModelMatrix(owner);
        Terathon::Point3D point(position.x, position.y, position.z);
        auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

        if (func) {
            func(*this, Vector3(localPosition.x, localPosition.y, localPosition.z));
        }
        return;
    }

    geo.position = position;
}

void WorldNodeTransform::SetWorldPositionXY(Vector3 position) {
    SetWorldPosition(position, [](WorldNodeTransform& transform, Vector3 localPos) {
        transform.geo.position = Vector3(localPos.x, localPos.y, transform.geo.position.z);
    });
}
