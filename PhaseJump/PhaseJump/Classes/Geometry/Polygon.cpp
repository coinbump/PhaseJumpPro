#include "Polygon.h"

using namespace std;
using namespace PJ;

/**
 NOTE: I'm not sure if this is a good algorithm. But it seems to work
 FUTURE: Evaluate better hit test algorithms if needed

 Reference: http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
 */
bool Polygon::TestHit(Vector2 pt) const {
    float testx = pt.x;
    float testy = pt.y;
    auto nvert = size();

    int c = 0;

    for (size_t i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((At(i).y > testy) != (At(j).y > testy)) &&
            (testx < (At(j).x - At(i).x) * (testy - At(i).y) / (At(j).y - At(i).y) + At(i).x)) {
            c = !c;
        }
    }

    return c != 0;
}
