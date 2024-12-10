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

        /// Add button with action to UI
        This& Button(String label, std::function<void()> func);

        /// Add text to UI
        This& Text(String label, std::function<String()> valueFunc);

        /// Add float input field to UI
        This& InputFloat(String label, Binding<float> binding);

        /// Add bool input field to UI
        This& InputBool(String label, Binding<bool> binding);

        /// Add text input field to UI
        This& InputText(String label, Binding<String> binding);

        /// Add single selection list UI
        This& ListSelect(String label, VectorList<String> choices, Binding<int> binding);

        /// Add list picker to UI
        This& PickerList(String label, VectorList<String> choices, Binding<int> binding);

        /// Add color picker field to UI
        This& PickerColor(String label, Binding<Color> binding);
    };
} // namespace PJ
