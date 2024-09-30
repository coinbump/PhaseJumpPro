#include "UIPlanner.h"
#include "Color.h"

using namespace std;
using namespace PJ;

using This = UIPlanner;

This& UIPlanner::InputFloat(String label, Binding<float> binding) {
    plan.Add(
        std::make_unique<ValueUIModel<float>>(UIModelId::InputFloat, label, ValueUICore{ binding })
    );
    return *this;
}

This& UIPlanner::InputColor(String label, Binding<Color> binding) {
    plan.Add(
        std::make_unique<ValueUIModel<Color>>(UIModelId::InputColor, label, ValueUICore{ binding })
    );
    return *this;
}

This& UIPlanner::InputBool(String label, Binding<bool> binding) {
    plan.Add(
        std::make_unique<ValueUIModel<bool>>(UIModelId::InputBool, label, ValueUICore{ binding })
    );
    return *this;
}

This& UIPlanner::InputText(String label, Binding<String> binding) {
    plan.Add(
        std::make_unique<ValueUIModel<String>>(UIModelId::InputText, label, ValueUICore{ binding })
    );
    return *this;
}

This& UIPlanner::PickerList(String label, VectorList<String> choices, Binding<int> binding) {
    plan.Add(std::make_unique<UIModel<ValueOptionsUICore>>(
        UIModelId::PickerList, label, ValueOptionsUICore{ choices, binding }
    ));
    return *this;
}
