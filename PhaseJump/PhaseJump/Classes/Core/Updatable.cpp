#include "Updatable.h"

using namespace std;
using namespace PJ;

void Updatable::SetIsFinished(bool value) {
    GUARD(isFinished != value)
    isFinished = value;

    GUARD(isFinished && onFinishFunc)
    onFinishFunc(*this);
}

Updatable::Updatable() {}

Updatable::Updatable(OnUpdateFunc onUpdateFunc) :
    onUpdateFunc(onUpdateFunc) {}

FinishType Updatable::OnUpdate(TimeSlice time) {
    GUARDR(!isFinished, FinishType::Finish)
    GUARDR(onUpdateFunc, FinishType::Continue)
    auto result = onUpdateFunc(*this, time);
    SetIsFinished(result == FinishType::Finish);

    return result;
}
