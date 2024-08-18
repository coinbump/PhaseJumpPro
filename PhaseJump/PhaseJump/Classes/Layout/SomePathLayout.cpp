#include "SomePathLayout.h"
#include "SomePath.h"
#include <stdio.h>

using namespace PJ;

void SomePathLayout::ApplyLayout() {
    GUARD(owner)

    auto path = BuildPath();
    auto owner = this->owner;

    auto childCount = owner->ChildCount();
    float normalOffset = childCount > 1 ? 1.0f / (float)(childCount - 1) : 0;

    auto index = 0;
    auto children = owner->ChildNodes();
    for (auto& childObject : children) {
        auto normalPosition = index * normalOffset;

        if (index < positions.size()) {
            normalPosition = positions[index];
        }

        auto position = path->PositionAt(normalPosition);
        position += offset;

        childObject->transform->SetLocalPosition(position);

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
                Angle(path->PositionAt(nextPosition) - path->PositionAt(prevPosition)).Degrees();
            rotationDegreeAngle += orientDegreeAngle;
            childObject->transform->SetRotation(Angle::DegreesAngle(rotationDegreeAngle));
        } else {
            childObject->transform->SetRotation(Angle::DegreesAngle(0));
        }

        index++;
    }
}
