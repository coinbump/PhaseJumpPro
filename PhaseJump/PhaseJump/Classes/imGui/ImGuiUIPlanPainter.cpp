#include "ImGuiUIPlanPainter.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

namespace {
    /// Cast result + transformed `const char*` option names for an options-based model.
    struct OptionsModel {
        CoreUIModel<ValueOptionsUICore>* model{};
        VectorList<char const*> names;
    };

    OptionsModel BuildOptionsModel(UIModel& _model) {
        OptionsModel result;
        result.model = dynamic_cast<CoreUIModel<ValueOptionsUICore>*>(&_model);
        GUARDR(result.model, result)

        auto& options = result.model->core.options;
        std::transform(
            options.begin(), options.end(), std::back_inserter(result.names),
            [](String const& str) { return str.c_str(); }
        );
        return result;
    }
} // namespace

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
        auto ptr = dynamic_cast<CoreUIModel<ActionUICore>*>(&_model);
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
            constexpr size_t minBufferSize = 10000;
            String modelValue = model.core.binding;

            VectorList<char> value;
            value.resize(std::max(minBufferSize, modelValue.size() + 1));

            std::copy(modelValue.begin(), modelValue.end(), value.begin());
            value[modelValue.size()] = '\0';

            if (ImGui::InputTextMultiline(model.name.c_str(), value.data(), value.size())) {
                String storageValue(value.begin(), std::find(value.begin(), value.end(), '\0'));
                model.core.binding = storageValue;
            }
        } catch (...) {}
    };

    drawModelFuncs[UIModelType::PickerList] = [](auto& painter, auto& _model) {
        auto optionsModel = BuildOptionsModel(_model);
        GUARD(optionsModel.model)
        auto& model = *optionsModel.model;

        int value = model.core.binding;
        if (ImGui::Combo(
                model.name.c_str(), &value, optionsModel.names.data(),
                (int)optionsModel.names.size(), (int)optionsModel.names.size()
            )) {
            model.core.binding = value;
        }
    };

    drawModelFuncs[UIModelType::Image] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<UIPlanner::ImageConfig>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto config = model.core.binding.getFunc();
        GUARD(config.imageId != 0 && config.size.x > 0 && config.size.y > 0)

        ImGui::Image(
            ImTextureRef((ImTextureID)(size_t)config.imageId), ImVec2(config.size.x, config.size.y),
            ImVec2(0, 1), ImVec2(1, 0)
        );
    };

    drawModelFuncs[UIModelType::ListSelect] = [](auto& painter, auto& _model) {
        auto optionsModel = BuildOptionsModel(_model);
        GUARD(optionsModel.model)
        auto& model = *optionsModel.model;

        int value = model.core.binding;

        // FUTURE: ImGui::PushID();
        ImGui::Text("%s", model.name.c_str());

        ImGui::SetNextItemWidth(-1.0f);

        String listBoxId = "##" + model.name;
        if (ImGui::ListBox(
                listBoxId.c_str(), &value, optionsModel.names.data(),
                (int)optionsModel.names.size(), 5
            )) {
            model.core.binding = value;
        }
        // FUTURE: ImGui::PopID();
    };
}
