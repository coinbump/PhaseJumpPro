#include "ImGuiPlanPainter.h"

using namespace std;
using namespace PJ;

ImGuiPlanPainter::ImGuiPlanPainter(UIPlan& plan, Storage& _storage) :
    storage(_storage),
    plan(plan) {
    drawFunc = [this](auto& painter) {
        for (auto& model : this->plan.Models()) {
            try {
                auto drawFunc = drawModelFuncs.at(model->classId);
                GUARD_CONTINUE(drawFunc)
                drawFunc(*this, *model.get());
            } catch (...) {}
        }
    };

    drawModelFuncs[UIModelId::InputFloat] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<float>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto label = model.name;
        model.core.value = model.core.binding;
        if (ImGui::InputFloat(model.name.c_str(), &model.core.value)) {
            model.core.binding = model.core.value;
        }
    };

    drawModelFuncs[UIModelId::PickerColor] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<Color>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        model.core.value = model.core.binding;
        if (ImGui::ColorEdit4(model.name.c_str(), &model.core.value.r)) {
            model.core.binding = model.core.value;
        }
    };

    drawModelFuncs[UIModelId::InputBool] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<bool>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        model.core.value = model.core.binding;
        if (ImGui::Checkbox(model.name.c_str(), &model.core.value)) {
            model.core.binding = model.core.value;
        }
    };

    drawModelFuncs[UIModelId::InputText] = [this](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<ValueUIModel<String>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        // FUTURE: re-evaluate this character limit as needed
        auto constexpr characterLimit = 2000;
        using TextStorage = std::array<char, characterLimit + 1>;

        try {
            auto& textStorage =
                storage.ValueStorage<TextStorage>(MakeString((uint64_t)ptr), "text");

            String modelValue = model.core.binding;
            std::copy(modelValue.begin(), modelValue.end(), textStorage.begin());
            textStorage[modelValue.size()] = 0;

            if (ImGui::InputTextMultiline(model.name.c_str(), textStorage.data(), characterLimit)) {
                String storageValue(
                    textStorage.begin(), std::find(textStorage.begin(), textStorage.end(), 0)
                );
                model.core.binding = storageValue;
            }
        } catch (...) {}
    };

    drawModelFuncs[UIModelId::PickerList] = [](auto& painter, auto& _model) {
        auto ptr = dynamic_cast<UIModel<ValueOptionsUICore>*>(&_model);
        GUARD(ptr)
        auto& model = *ptr;

        auto& options = model.core.options;

        VectorList<const char*> optionNames;
        std::transform(
            options.begin(), options.end(), std::back_inserter(optionNames),
            [](String& str) { return str.c_str(); }
        );

        try {
            auto& storage = model.core.value;
            storage = model.core.binding;

            if (ImGui::Combo(
                    model.name.c_str(), &storage, optionNames.data(), (int)optionNames.size(),
                    (int)optionNames.size()
                )) {

                model.core.binding = storage;
            }
        } catch (...) {}
    };
}
