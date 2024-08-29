#include "CenterPolyBuilder.h"

using namespace std;
using namespace PJ;

CenterPolyShape CenterPolyShape::circle(
    Angle::DegreesAngle(0), Angle::DegreesAngle(360), Angle::DegreesAngle(10)
);
CenterPolyShape
    CenterPolyShape::hex(Angle::DegreesAngle(0), Angle::DegreesAngle(360), Angle::DegreesAngle(60));
CenterPolyShape CenterPolyShape::square(
    Angle::DegreesAngle(-45), Angle::DegreesAngle(360 - 45), Angle::DegreesAngle(90)
);
