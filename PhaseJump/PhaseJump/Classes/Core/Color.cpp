#include "Color.h"
#include "PropertyIdBuilder.h"

using namespace std;
using namespace PJ;

const Color Color::red(1.0f, 0, 0, 1.0f);
const Color Color::green(0, 1.0f, 0, 1.0f);
const Color Color::blue(0, 0, 1.0f, 1.0f);

const Color Color::black(0, 0, 0, 1.0f);
const Color Color::clear(0, 0, 0.0f, 0.0f);
const Color Color::gray(0.5f, 0.5f, 0.5f, 1.0f);
const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::yellow(1.0f, 1.0f, 0, 1.0f);

ostream& PJ::operator<<(ostream& os, Color const& value) {
    std::array<float, 4> values{ value.r, value.g, value.b, value.a };
    os << PropertyIdBuilder().AddCollection("", values).Result();
    return os;
}
