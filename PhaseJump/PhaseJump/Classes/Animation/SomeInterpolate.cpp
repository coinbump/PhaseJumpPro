#include "SomeInterpolate.h"

#include <memory>

using namespace std;
using namespace PJ;

const std::shared_ptr<SomeInterpolate> _InterpolateType::linear = make_shared<LinearInterpolate>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeInSquared = make_shared<EaseInSquared>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeInCubed = make_shared<EaseInCubed>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeOutSquared = make_shared<EaseOutSquared>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeOutCubed = make_shared<EaseOutCubed>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeOutElastic = make_shared<EaseOutElastic>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeInBack = make_shared<EaseInBack>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeOutBack = make_shared<EaseOutBack>();
const std::shared_ptr<SomeInterpolate> _InterpolateType::easeInOutBack = make_shared<EaseInOutBack>();
