#include "UIPlanner.h"
#include "Color.h"

using namespace std;
using namespace PJ;

using This = UIPlanner;

This& UIPlanner::Text(std::function<TextConfig()> configFunc) {
    GUARDR(configFunc, *this)

    plan.Add([=]() {
        auto config = configFunc();
        Binding<String> binding([=]() { return config.text; }, {});
        return NEW<ValueUIModel<String>>(UIModelType::Text, config.label, ValueUICore{ binding });
    });
    return *this;
}

This& UIPlanner::TreeNode(std::function<TreeNodeConfig()> configFunc) {
    GUARDR(configFunc, *this)

    plan.Add([=]() {
        Binding<TreeNodeConfig> binding([=]() { return configFunc(); }, {});
        return NEW<ValueUIModel<TreeNodeConfig>>(UIModelType::TreeNode, "", ValueUICore{ binding });
    });
    return *this;
}

This& UIPlanner::Button(std::function<ButtonConfig()> configFunc) {
    GUARDR(configFunc, *this)

    plan.Add([=]() {
        auto config = configFunc();
        return NEW<UIModel<ActionUICore>>(
            UIModelType::Button, config.label, ActionUICore{ .func = config.func }
        );
    });
    return *this;
}

This& UIPlanner::InputFloat(InputFloatConfig config) {
    plan.Add([=]() {
        return NEW<ValueUIModel<float>>(
            UIModelType::InputFloat, config.label, ValueUICore{ config.binding }
        );
    });
    return *this;
}

This& UIPlanner::PickerColor(PickerColorConfig config) {
    plan.Add([=]() {
        return NEW<ValueUIModel<Color>>(
            UIModelType::PickerColor, config.label, ValueUICore{ config.binding }
        );
    });
    return *this;
}

This& UIPlanner::InputBool(InputBoolConfig config) {
    plan.Add([=]() {
        return NEW<ValueUIModel<bool>>(
            UIModelType::InputBool, config.label, ValueUICore{ config.binding }
        );
    });
    return *this;
}

This& UIPlanner::InputText(InputTextConfig config) {
    plan.Add([=]() {
        return NEW<ValueUIModel<String>>(
            UIModelType::InputText, config.label, ValueUICore{ config.binding }
        );
    });
    return *this;
}

This& UIPlanner::PickerList(std::function<PickerListConfig()> configFunc) {
    GUARDR(configFunc, *this)

    plan.Add([=]() {
        auto config = configFunc();

        return NEW<UIModel<ValueOptionsUICore>>(
            UIModelType::PickerList, config.label,
            ValueOptionsUICore{ config.options, config.binding }
        );
    });
    return *this;
}

This& UIPlanner::ListSelect(std::function<ListSelectConfig()> configFunc) {
    GUARDR(configFunc, *this)

    plan.Add([=]() {
        auto config = configFunc();

        return NEW<UIModel<ValueOptionsUICore>>(
            UIModelType::ListSelect, config.label,
            ValueOptionsUICore{ config.options, config.binding }
        );
    });
    return *this;
}
