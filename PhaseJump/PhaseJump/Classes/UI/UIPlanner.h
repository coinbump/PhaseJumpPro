#pragma once

#include "UIPlan.h"

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

        /// Add float input field to UI
        This& InputFloat(String label, Binding<float> binding);

        /// Add color input field to UI
        This& InputColor(String label, Binding<Color> binding);

        /// Add bool input field to UI
        This& InputBool(String label, Binding<bool> binding);

        /// Add text input field to UI
        This& InputText(String label, Binding<String> binding);

        /// Add list picker to UI
        This& PickerList(String label, VectorList<String> choices, Binding<int> binding);
    };
} // namespace PJ
