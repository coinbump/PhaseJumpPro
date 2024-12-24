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

void Updatable::Finish() {
    SetIsFinished(true);
}

FinishType Updatable::Update(TimeSlice time) {
    OnUpdate(time);
    return isFinished ? FinishType::Finish : FinishType::Continue;
}

void Updatable::OnUpdate(TimeSlice time) {
    GUARD(!isFinished)
    GUARD(onUpdateFunc)
    SetIsFinished(onUpdateFunc(*this, time) == FinishType::Finish);
}
