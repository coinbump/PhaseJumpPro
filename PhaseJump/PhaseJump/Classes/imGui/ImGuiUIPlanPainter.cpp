#include "ImGuiUIPlanPainter.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

ImGuiUIPlanPainter::ImGuiUIPlanPainter(UIPlan& plan) :
    plan(plan) {
    drawFunc = [this](auto& painter) {
        for (auto& modelFunc : this->plan.ModelFuncs()) {
            auto model = modelFunc();

            try {
                auto drawFunc = drawModelFuncs.at(model->classId);
                GUARD_CONTINUE(drawFunc)
                drawFunc(*this, *model.get());
            } catch (...) {}
        }
    };

    drawModelFuncs[UIModelType::Button] = [this](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<UIModel<ActionUICore>*>(&_model);
        GUARD(ptr);
        auto& model = *ptr;

        auto label = model.name;

        if (!IsEmpty(label)) {
            if (ImGui::Button(label.c_str())) {
                GUARD(model.core.func)
                model.core.func();
            }
        }
    };

    drawModelFuncs[UIModelType::Text] = [this](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<String>*>(&_model);
        GUARD(ptr);
        auto& model = *ptr;

        auto label = model.name;
        String value = model.core.binding;

        if (!IsEmpty(value)) {
            if (!IsEmpty(label)) {
                ImGui::Text("%s", label.c_str());
            }
            ImGui::Text("%s", value.c_str());
        }
    };

    drawModelFuncs[UIModelType::TreeNode] = [this](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<UIPlanner::TreeNodeConfig>*>(&_model);
        GUARD(ptr);
        auto& model = *ptr;
        auto config = model.core.binding.getFunc();
        auto label = config.label;

        if (ImGui::TreeNodeEx(
                label.c_str(), config.isOpenDefault ? ImGuiTreeNodeFlags_DefaultOpen : 0
            )) {
            if (config.planUIFunc) {
                UIPlan childPlan;
                UIPlanner childPlanner(childPlan);
                config.planUIFunc(childPlanner);
                ImGuiUIPlanPainter childPainter(childPlan);
                childPainter.Draw();
            }

            ImGui::TreePop();
        }
    };

    drawModelFuncs[UIModelType::InputFloat] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<float>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto label = model.name;
        float value = model.core.binding;
        if (ImGui::InputFloat(model.name.c_str(), &value)) {
            model.core.binding = value;
        }
    };

    drawModelFuncs[UIModelType::PickerColor] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<Color>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        Color value = model.core.binding;
        if (ImGui::ColorEdit4(model.name.c_str(), &value.r)) {
            model.core.binding = value;
        }
    };

    drawModelFuncs[UIModelType::InputBool] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<bool>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        bool value = model.core.binding;
        if (ImGui::Checkbox(model.name.c_str(), &value)) {
            model.core.binding = value;
        }
    };

    drawModelFuncs[UIModelType::InputText] = [this](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<String>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        try {
            String modelValue = model.core.binding;
            VectorList<char> value;
            value.resize(modelValue.size() + 16);

            std::copy(modelValue.begin(), modelValue.end(), value.begin());

            if (ImGui::InputTextMultiline(model.name.c_str(), value.data(), 10000)) {
                String storageValue(value.begin(), std::find(value.begin(), value.end(), 0));
                model.core.binding = storageValue;
            }
        } catch (...) {}
    };

    drawModelFuncs[UIModelType::PickerList] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<UIModel<ValueOptionsUICore>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto& options = model.core.options;

        VectorList<const char*> optionNames;
        std::transform(
            options.begin(), options.end(), std::back_inserter(optionNames),
            [](String& str) { return str.c_str(); }
        );

        int value = model.core.binding;

        if (ImGui::Combo(
                model.name.c_str(), &value, optionNames.data(), (int)optionNames.size(),
                (int)optionNames.size()
            )) {
            model.core.binding = value;
        }
    };

    drawModelFuncs[UIModelType::ListSelect] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<UIModel<ValueOptionsUICore>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto& options = model.core.options;

        VectorList<const char*> optionNames;
        std::transform(
            options.begin(), options.end(), std::back_inserter(optionNames),
            [](String& str) { return str.c_str(); }
        );

        int value = model.core.binding;

        // FUTURE: ImGui::PushID();
        if (ImGui::ListBox(
                model.name.c_str(), &value, optionNames.data(), (int)optionNames.size(), 5
            )) {
            model.core.binding = value;
        }
        // FUTURE: ImGui::PopID();
    };
}
