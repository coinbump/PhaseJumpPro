#include "SomeInterpolate.h"

#include "Macros.h"
#include <memory>

using namespace std;
using namespace PJ;

const SP<SomeInterpolate> InterpolateTypes::linear = MAKE<InterpolateLinear>();
const SP<SomeInterpolate> InterpolateTypes::easeInSquared = MAKE<InterpolateEaseInSquared>();
const SP<SomeInterpolate> InterpolateTypes::easeInCubed = MAKE<InterpolateEaseInCubed>();
const SP<SomeInterpolate> InterpolateTypes::easeOutSquared = MAKE<InterpolateEaseOutSquared>();
const SP<SomeInterpolate> InterpolateTypes::easeOutCubed = MAKE<InterpolateEaseOutCubed>();
const SP<SomeInterpolate> InterpolateTypes::easeOutElastic = MAKE<InterpolateEaseOutElastic>();
const SP<SomeInterpolate> InterpolateTypes::easeInBack = MAKE<InterpolateEaseInBack>();
const SP<SomeInterpolate> InterpolateTypes::easeOutBack = MAKE<InterpolateEaseOutBack>();
const SP<SomeInterpolate> InterpolateTypes::easeInOutBack = MAKE<InterpolateEaseInOutBack>();
