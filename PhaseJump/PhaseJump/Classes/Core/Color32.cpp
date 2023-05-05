#include "Color32.h"

using namespace std;
using namespace PJ;

const ComponentColorSchema ComponentColorSchema::rgba(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 8, 16, 24);
const ComponentColorSchema ComponentColorSchema::bgra(0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000, 16, 8, 0, 24);
