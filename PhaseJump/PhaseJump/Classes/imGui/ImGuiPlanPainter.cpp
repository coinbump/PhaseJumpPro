#include "ImGuiPlanPainter.h"

using namespace std;
using namespace PJ;

ImGuiPlanPainter::ImGuiPlanPainter() {
    auto constexpr storageId = "imGui.storage";

    drawFuncs[UIModelId::InputFloat] = [](auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<float>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto label = model.label;
        model.core.value = model.core.binding;
        if (ImGui::InputFloat(model.label.c_str(), &model.core.value)) {
            model.core.binding = model.core.value;
        }
    };

    drawFuncs[UIModelId::InputColor] = [](auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<Color>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        model.core.value = model.core.binding;
        if (ImGui::ColorEdit4(model.label.c_str(), &model.core.value.r)) {
            model.core.binding = model.core.value;
        }
    };

    drawFuncs[UIModelId::InputBool] = [](auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<bool>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        model.core.value = model.core.binding;
        if (ImGui::Checkbox(model.label.c_str(), &model.core.value)) {
            model.core.binding = model.core.value;
        }
    };

    drawFuncs[UIModelId::InputText] = [](auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<String>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        // FUTURE: re-evaluate this character limit as needed
        auto constexpr characterLimit = 2000;
        using Storage = std::array<char, characterLimit>;

        try {
            model.tags.TypeAddIfMissing<Storage>(storageId);
            Storage& storage = model.tags.TypeValueAt<Storage>(storageId);

            String modelValue = model.core.binding;
            std::copy(modelValue.begin(), modelValue.end(), storage.begin());
            storage[modelValue.size()] = 0;

            if (ImGui::InputTextMultiline(model.label.c_str(), storage.data(), characterLimit)) {
                std::string storageValue(storage.begin(), storage.end());
                model.core.binding = storageValue;
            }
        } catch (...) {}
    };

    drawFuncs[UIModelId::PickerList] = [](auto& _model) {
        auto ptr = dynamic_cast<UIModel<ValueOptionsUICore>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        using Storage = int;

        auto& options = model.core.options;

        VectorList<const char*> optionNames;
        std::transform(
            options.begin(), options.end(), std::back_inserter(optionNames),
            [](String& str) { return str.c_str(); }
        );

        try {
            Storage& storage = model.core.value;
            storage = model.core.binding;

            if (ImGui::Combo(
                    model.label.c_str(), &storage, optionNames.data(), (int)optionNames.size(),
                    (int)optionNames.size()
                )) {

                model.core.binding = storage;
            }
        } catch (...) {}
    };
}

void ImGuiPlanPainter::Draw(UIPlan& plan) {
    for (auto& model : plan.Models()) {
        try {
            auto drawFunc = drawFuncs.at(model->classId);
            GUARD_CONTINUE(drawFunc)
            drawFunc(*model.get());
        } catch (...) {}
    }
}
