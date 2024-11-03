#pragma once

#include "Color.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    /// Common ids for controller board values
    namespace ControllerBoardId {
        /// Special case: used for a set func that is called for all value changes, regardless of id
        auto constexpr All = ".*";
    } // namespace ControllerBoardId

    /// Central point of control for elements such as switches, effects, colors, lighting, etc.
    class ControllerBoard : public WorldComponent<> {
    public:
        using This = ControllerBoard;

        /// Function that sets the value
        template <class Type>
        using SetFunc = std::function<void(String id, Type)>;

        /// Map of set funcs
        template <class Type>
        using SetFuncMap = UnorderedMap<String, SetFunc<Type>>;

        /// Map of values
        template <class Type>
        using ValueMap = UnorderedMap<String, Type>;

    protected:
        ValueMap<Color> colors;
        ValueMap<bool> switches;
        ValueMap<String> states;
        ValueMap<float> values;

        template <class Type>
        void Set(String id, Type value, ValueMap<Type>& values, SetFuncMap<Type>& funcs) {
            try {
                auto& oldValue = values[id];
                GUARD(oldValue != value)
            } catch (...) {}

            values.insert_or_assign(id, value);

            try {
                auto& func = funcs.at(id);
                func(id, value);
            } catch (...) {}

            // Special case: func for all ids
            try {
                auto& func = funcs.at(ControllerBoardId::All);
                func(id, value);
            } catch (...) {}
        }

    public:
        SetFuncMap<Color> colorFuncs;
        SetFuncMap<bool> switchFuncs;
        SetFuncMap<String> stateFuncs;
        SetFuncMap<float> valueFuncs;

        void SetColor(String id, Color value) {
            Set(id, value, colors, colorFuncs);
        }

        void SetSwitch(String id, bool value) {
            Set(id, value, switches, switchFuncs);
        }

        void SetState(String id, String value) {
            Set(id, value, states, stateFuncs);
        }

        void SetValue(String id, float value) {
            Set(id, value, values, valueFuncs);
        }
    };
} // namespace PJ
