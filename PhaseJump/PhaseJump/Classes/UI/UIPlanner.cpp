#include "UIPlanner.h"
#include "Color.h"

using namespace std;
using namespace PJ;

using This = UIPlanner;

This& UIPlanner::InputFloat(String label, Binding<float> binding) {
    plan.Add(NEW<ValueUIModel<float>>(UIModelId::InputFloat, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::PickerColor(String label, Binding<Color> binding) {
    plan.Add(NEW<ValueUIModel<Color>>(UIModelId::PickerColor, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::InputBool(String label, Binding<bool> binding) {
    plan.Add(NEW<ValueUIModel<bool>>(UIModelId::InputBool, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::InputText(String label, Binding<String> binding) {
    plan.Add(NEW<ValueUIModel<String>>(UIModelId::InputText, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::PickerList(String label, VectorList<String> choices, Binding<int> binding) {
    plan.Add(NEW<UIModel<ValueOptionsUICore>>(
        UIModelId::PickerList, label, ValueOptionsUICore{ choices, binding }
    ));
    return *this;
}
