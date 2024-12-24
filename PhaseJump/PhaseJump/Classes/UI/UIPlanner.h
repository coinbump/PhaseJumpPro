#pragma once

#include "UIPlan.h"
#include "UITypes.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    class Color;

    class UIPlanner {
    public:
        using This = UIPlanner;

        UIPlan& plan;

        UIPlanner(UIPlan& plan) :
            plan(plan) {}

        struct ButtonConfig {
            String label;
            std::function<void()> func;
        };

        struct TextConfig {
            String label;
            String text;
        };

        struct InputFloatConfig {
            String label;
            Binding<float> binding;
        };

        struct InputBoolConfig {
            String label;
            Binding<bool> binding;
        };

        struct InputTextConfig {
            String label;
            Binding<String> binding;
        };

        struct ListSelectConfig {
            String label;
            VectorList<String> options;
            Binding<int> binding;
        };

        struct PickerListConfig {
            String label;
            VectorList<String> options;
            Binding<int> binding;
        };

        struct PickerColorConfig {
            String label;
            Binding<Color> binding;
        };

        // FUTURE: add dynamic builders as needed

        /// Adds button with action to UI
        This& Button(std::function<ButtonConfig()> configFunc);

        /// Adds button with action to UI
        This& Button(ButtonConfig config) {
            return Button([=]() { return config; });
        }

        /// Adds dynamic text to UI
        This& Text(std::function<TextConfig()> configFunc);

        /// Adds text to UI
        This& Text(TextConfig config) {
            return Text([=]() { return config; });
        }

        /// Adds float input field to UI
        This& InputFloat(InputFloatConfig config);

        /// Adds bool input field to UI
        This& InputBool(InputBoolConfig config);

        /// Adds text input field to UI
        This& InputText(InputTextConfig config);

        /// Adds dynamic single selection list UI
        This& ListSelect(std::function<ListSelectConfig()> configFunc);

        /// Adds single selection list UI
        This& ListSelect(ListSelectConfig config) {
            return ListSelect([=]() { return config; });
        }

        /// Adds dynamic list picker to UI
        This& PickerList(std::function<PickerListConfig()> configFunc);

        /// Adds list picker to UI
        This& PickerList(PickerListConfig config) {
            return PickerList([=]() { return config; });
        }

        /// Adds color picker field to UI
        This& PickerColor(PickerColorConfig config);
    };
} // namespace PJ
