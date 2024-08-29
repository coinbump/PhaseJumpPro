#include "SomeControllerUIEvent.h"

using namespace std;
using namespace PJ;

VectorList<String> ControllerAxisId::allCases{
    ControllerAxisId::LeftX,  ControllerAxisId::LeftY,       ControllerAxisId::RightX,
    ControllerAxisId::RightY, ControllerAxisId::LeftTrigger, ControllerAxisId::RightTrigger
};
