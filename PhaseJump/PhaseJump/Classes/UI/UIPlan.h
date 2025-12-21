#pragma once

#include "Binding.h"
#include "Tags.h"
#include "TreeNode.h"
#include "VectorList.h"
#include "Void.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    class UIModel;

    /// Class ids for UI models in a UI plan
    namespace UIModelType {
        auto constexpr InputFloat = "input.float";
        auto constexpr InputBool = "input.bool";
        auto constexpr InputInt = "input.int";
        auto constexpr InputText = "input.text";
        auto constexpr PickerList = "picker.list";
        auto constexpr ListSelect = "list.select";
        auto constexpr PickerColor = "picker.color";
        auto constexpr Text = "text";
        auto constexpr TreeNode = "tree.node";
        auto constexpr Button = "button";
    } // namespace UIModelType

    /// An item in a UI plan, used to build UIs
    class UIModel : public Treeable<UIModel> {
    public:
        using This = UIModel;
        using TreeNode = TreeNode<This, UP<This>>;

        /// Child models for hierarchical UI
        TreeNode tree;

        /// Class id for the type of UI item this represents
        /// Example: "button", "slider", etc.
        String classId;

        /// Name to show to the user (if UI component has a name label)
        String name;

        /// Custom properties
        Tags tags;

        UIModel(String classId, String label) :
            tree(*this),
            classId(classId),
            name(label) {}

        virtual ~UIModel() {}

        // MARK: Treeable

        This* Parent() const override {
            return tree.Parent();
        }

        void SetParent(This* value) override {
            tree.SetParent(value);
        }
    };

    /**
     Plan for building a UI. Abstracts the idea of how a UI operates from its implementation
     Allows us to create a model that describes the UI and resolve that plan later by something like
     imGUI, or even our own custom UI logic
     */
    class UIPlan {
    public:
        using This = UIPlan;

        using ModelFunc = std::function<UP<UIModel>()>;

    protected:
        VectorList<ModelFunc> modelFuncs;

    public:
        VectorList<ModelFunc> const& ModelFuncs() const {
            return modelFuncs;
        }

        void Add(ModelFunc modelFunc) {
            modelFuncs.push_back(modelFunc);
        }
    };

    /// UI plan model with core
    template <class Core = Void>
    class CoreUIModel : public UIModel {
    public:
        using Base = UIModel;
        using This = CoreUIModel;

        Core core{};

        CoreUIModel(String classId, String label, Core core = {}) :
            Base(classId, label),
            core(core) {}
    };

    /// Core for UI plan model with an action
    class ActionUICore {
    public:
        std::function<void()> func;
    };

    /// Core for UI plan model with a storage value and binding
    template <class Type>
    class ValueUICore {
    public:
        using This = ValueUICore;

        Binding<Type> binding;

        ValueUICore(Binding<Type> binding) :
            binding(binding) {}
    };

    /// UI plan item with value storage and binding
    template <class Type>
    using ValueUIModel = CoreUIModel<ValueUICore<Type>>;

    /// UI model core with a list of value choices and a chosen value
    class ValueOptionsUICore {
    public:
        using This = ValueOptionsUICore;

        /// List of possible values
        VectorList<String> options;

        Binding<int> binding;

        ValueOptionsUICore(VectorList<String> options, Binding<int> binding) :
            options(options),
            binding(binding) {}
    };
} // namespace PJ
