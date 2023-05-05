#include "SomeInterpolate.h"

#include "Macros.h"
#include <memory>

using namespace std;
using namespace PJ;

const SP<SomeInterpolate> _InterpolateType::linear = MAKE<LinearInterpolate>();
const SP<SomeInterpolate> _InterpolateType::easeInSquared = MAKE<EaseInSquared>();
const SP<SomeInterpolate> _InterpolateType::easeInCubed = MAKE<EaseInCubed>();
const SP<SomeInterpolate> _InterpolateType::easeOutSquared = MAKE<EaseOutSquared>();
const SP<SomeInterpolate> _InterpolateType::easeOutCubed = MAKE<EaseOutCubed>();
const SP<SomeInterpolate> _InterpolateType::easeOutElastic = MAKE<EaseOutElastic>();
const SP<SomeInterpolate> _InterpolateType::easeInBack = MAKE<EaseInBack>();
const SP<SomeInterpolate> _InterpolateType::easeOutBack = MAKE<EaseOutBack>();
const SP<SomeInterpolate> _InterpolateType::easeInOutBack = MAKE<EaseInOutBack>();
