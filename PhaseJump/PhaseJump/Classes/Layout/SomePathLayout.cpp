#include "SomePathLayout.h"
#include "SomePath.h"
#include "WorldNode.h"
#include <stdio.h>

using namespace PJ;

void SomePathLayout::ApplyLayout() {
    GUARD(owner)

    auto path = BuildPath();

    auto childCount = owner->ChildCount();
    float normalOffset = childCount > 1 ? 1.0f / (float)(childCount - 1) : 0;

    auto index = 0;
    auto children = owner->ChildNodes();
    for (auto& childObject : children) {
        auto normalPosition = index * normalOffset;

        if (index < positions.size()) {
            normalPosition = positions[index];
        }

        auto position = path->PositionAt(normalPosition) + offset;
        childObject->SetLocalPosition(position);

        if (orientToPath) {
            auto prevPosition = normalPosition;
            auto nextPosition = normalPosition;
            auto orientDelta = .001f;
            if (normalPosition == 0) {
                nextPosition += orientDelta;
            } else if (normalPosition == 1.0f) {
                prevPosition -= orientDelta;
            } else {
                nextPosition = std::min(1.0f, nextPosition + orientDelta);
            }

            auto rotationDegreeAngle =
                Angle(path->PositionAt(nextPosition) - path->PositionAt(prevPosition));
            rotationDegreeAngle += orientDegreeAngle;
            childObject->transform.SetRotation(rotationDegreeAngle);
        } else {
            childObject->transform.SetRotation(Angle::DegreesAngle(0));
        }

        index++;
    }
}
