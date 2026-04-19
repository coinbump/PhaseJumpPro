#pragma once

#include "UIPlan.h"
#include "UITypes.h"
#include "Vector2.h"

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

        struct LabelTextConfig {
            String label;
            String text;
        };

        struct ImageConfig {
            RenderItemId imageId{};
            Vector2 size;
        };

        struct TreeNodeConfig {
            using PlanUIFunc = std::function<void(UIPlanner&)>;

            String label;
            bool isOpenDefault{};
            PlanUIFunc planUIFunc;
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

        /// Adds dynamic label and text to UI
        This& LabelText(std::function<LabelTextConfig()> configFunc);

        /// Adds label and text to UI
        This& LabelText(LabelTextConfig config) {
            return LabelText([=]() { return config; });
        }

        /// Adds dynamic tree node to UI
        This& TreeNode(std::function<TreeNodeConfig()> configFunc);

        /// Adds tree node to UI
        This& TreeNode(TreeNodeConfig config) {
            return TreeNode([=]() { return config; });
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

        /// Adds static list picker to UI
        This& PickerList(PickerListConfig config) {
            return PickerList([=]() { return config; });
        }

        /// Adds color picker field to UI
        This& PickerColor(PickerColorConfig config);

        /// Adds dynamic image to UI
        This& Image(std::function<ImageConfig()> configFunc);

        /// Adds static image to UI
        This& Image(ImageConfig config) {
            return Image([=]() { return config; });
        }
    };
} // namespace PJ
