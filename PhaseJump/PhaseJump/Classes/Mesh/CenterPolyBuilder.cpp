#include "CenterPolyBuilder.h"

using namespace std;
using namespace PJ;

CenterPolyModel CenterPolyModel::ellipse{ .angleStep = Angle::WithDegrees(10) };
CenterPolyModel CenterPolyModel::hex{ .angleStep = Angle::WithDegrees(60) };
CenterPolyModel CenterPolyModel::quad{ .angleStep = Angle::WithDegrees(90) };

Polygon CenterPolyBuilder::Build(Vector2 center, Vector2 worldSize, CenterPolyModel shape) {
    Polygon poly;

    auto radii = worldSize / 2.0f;

    auto endAngleDegrees =
        shape.startAngle.Degrees() + std::min(360.0f, shape.angleDelta.Degrees());

    float angleDelta = shape.angleDelta.Degrees();
    float angleStep = shape.angleStep.Degrees();

    // Avoid infinite loop
    if (angleStep <= 0 || angleDelta < 0) {
        return poly;
    }

    float angle = shape.startAngle.Degrees();
    float angleAdded = angle;

    while (angle < endAngleDegrees) {
        Vector2 vector = Angle::WithDegrees(angle);
        Vector2 vertex(center.x + vector.x * radii.x, center.y + vector.y * radii.y);
        poly.Add(vertex);
        angleAdded = angle;

        angle += angleStep;
    }

    // Add final step if we've gone past the end angle and we didn't add it
    // Closed shapes don't need this
    if (!shape.IsClosed()) {
        if (angleAdded < endAngleDegrees) {
            Vector2 vector = Angle::WithDegrees(endAngleDegrees);
            Vector2 vertex(center.x + vector.x * radii.x, center.y + vector.y * radii.y);
            poly.Add(vertex);
        }
    }

    return poly;
}
