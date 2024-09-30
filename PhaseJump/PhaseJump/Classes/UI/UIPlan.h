#pragma once

#include "Binding.h"
#include "Tags.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    class SomeUIModel;

    /// Class ids for UI models in a UI plan
    namespace UIModelId {
        auto constexpr InputFloat = "input.float";
        auto constexpr InputColor = "input.color";
        auto constexpr InputBool = "input.bool";
        auto constexpr InputInt = "input.int";
        auto constexpr InputText = "input.text";
        auto constexpr PickerList = "picker.list";
    } // namespace UIModelId

    /// An item in a UI plan, used to build UIs
    class SomeUIModel {
    public:
        using This = SomeUIModel;

        /// Class id for the type of UI item this represents
        /// Example: "button", "slider", etc.
        String classId;

        /// Label to show to the user
        String label;

        /// Custom properties
        Tags tags;

        /// Child models for hierarchical UI
        VectorList<UP<SomeUIModel>> children;

        SomeUIModel(String classId, String label) :
            classId(classId),
            label(label) {}

        virtual ~SomeUIModel() {}
    };

    /**
     Plan for building a UI. Abstracts the idea of how a UI operates from its implementation
     Allows us to create a model that describes the UI and resolve that plan later by something like
     imGUI, or even our own custom UI logic
     */
    class UIPlan {
    public:
        using This = UIPlan;

    protected:
        VectorList<UP<SomeUIModel>> models;

    public:
        VectorList<UP<SomeUIModel>> const& Models() const {
            return models;
        }

        void Add(UP<SomeUIModel>&& model) {
            models.push_back(std::move(model));
        }
    };

    /// UI plan model with core
    template <class Core>
    class UIModel : public SomeUIModel {
    public:
        using Base = SomeUIModel;
        using This = UIModel;

        Core core{};

        UIModel(String classId, String label, Core core = {}) :
            Base(classId, label),
            core(core) {}
    };

    /// Core for UI plan model with a storage value and binding
    template <class Type>
    class ValueUICore {
    public:
        using This = ValueUICore;

        /// Storage for value, if needed (for immediate mode UI)
        Type value{};

        Binding<Type> binding;

        ValueUICore(Binding<Type> binding) :
            binding(binding) {}
    };

    /// UI plan item with value storage and binding
    template <class Type>
    using ValueUIModel = UIModel<ValueUICore<Type>>;

    /// UI model core with a list of value choices and a chosen value
    class ValueOptionsUICore {
    public:
        using This = ValueOptionsUICore;

        /// List of possible values
        VectorList<String> options;

        /// Storage for value, if needed (for immediate mode UI)
        int value{};

        Binding<int> binding;

        ValueOptionsUICore(VectorList<String> options, Binding<int> binding) :
            options(options),
            binding(binding) {}
    };
} // namespace PJ
