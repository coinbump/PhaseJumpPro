#include "Polygon.h"

using namespace std;
using namespace PJ;

Vector2 Polygon::CalculateMin() const {
    GUARDR(!IsEmpty(value), {})

    auto result = value[0];

    for (auto& vertex : value) {
        result = Vector2(std::min(result.x, vertex.x), std::min(result.y, vertex.y));
    }

    return result;
}

Vector2 Polygon::CalculateMax() const {
    GUARDR(!IsEmpty(value), {})

    auto result = value[0];

    for (auto& vertex : value) {
        result = { std::max(result.x, vertex.x), std::max(result.y, vertex.y) };
    }

    return result;
}

/**
 NOTE: I'm not sure if this is a good algorithm. But it seems to work
 FUTURE: Evaluate better hit test algorithms if needed

 Reference:
 http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
 */
// TODO: needs unit tests
bool Polygon::TestHit(Vector2 pt) const {
    float testx = pt.x;
    float testy = pt.y;
    auto nvert = value.size();

    int c = 0;

    for (size_t i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((ModAt(i).y > testy) != (ModAt(j).y > testy)) &&
            (testx < (ModAt(j).x - ModAt(i).x) * (testy - ModAt(i).y) / (ModAt(j).y - ModAt(i).y) +
                         ModAt(i).x)) {
            c = !c;
        }
    }

    return c != 0;
}

String Polygon::ToString() const {
    stringstream s;
    s << "[";
    for (auto& v : value) {
        s << v.ToString() << ", ";
    }
    s << "]";

    return s.str();
}

void Polygon::SetSize(Vector2 value) {
    // Once a polygon is zero size, it can never be resized
    GUARD(value.x > 0 && value.y > 0)

    auto center = Center();
    auto size = Size();
    GUARD(size != value);

    // Avoid divide by zero
    GUARD(size.x > 0 && size.y > 0)

    auto factor = value / size;

    for (auto& v : this->value) {
        auto distance = v - center;
        distance *= factor;
        v = center + distance;
    }

    isBoundsInvalid = true;
}
