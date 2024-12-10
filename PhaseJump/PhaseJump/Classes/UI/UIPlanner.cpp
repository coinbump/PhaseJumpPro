#include "UIPlanner.h"
#include "Color.h"

using namespace std;
using namespace PJ;

using This = UIPlanner;

This& UIPlanner::Text(String label, std::function<String()> valueFunc) {
    Binding<String> binding(valueFunc, {});
    plan.Add(NEW<ValueUIModel<String>>(UIModelType::Text, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::Button(String label, std::function<void()> func) {
    plan.Add(NEW<UIModel<ActionUICore>>(UIModelType::Button, label, ActionUICore{ .func = func }));
    return *this;
}

This& UIPlanner::InputFloat(String label, Binding<float> binding) {
    plan.Add(NEW<ValueUIModel<float>>(UIModelType::InputFloat, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::PickerColor(String label, Binding<Color> binding) {
    plan.Add(NEW<ValueUIModel<Color>>(UIModelType::PickerColor, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::InputBool(String label, Binding<bool> binding) {
    plan.Add(NEW<ValueUIModel<bool>>(UIModelType::InputBool, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::InputText(String label, Binding<String> binding) {
    plan.Add(NEW<ValueUIModel<String>>(UIModelType::InputText, label, ValueUICore{ binding }));
    return *this;
}

This& UIPlanner::PickerList(String label, VectorList<String> choices, Binding<int> binding) {
    plan.Add(NEW<UIModel<ValueOptionsUICore>>(
        UIModelType::PickerList, label, ValueOptionsUICore{ choices, binding }
    ));
    return *this;
}

This& UIPlanner::ListSelect(String label, VectorList<String> choices, Binding<int> binding) {
    plan.Add(NEW<UIModel<ValueOptionsUICore>>(
        UIModelType::ListSelect, label, ValueOptionsUICore{ choices, binding }
    ));
    return *this;
}
