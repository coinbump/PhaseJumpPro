#pragma once

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/15/26
 */
namespace PJ {
    enum class SetOnValueChangeFuncType {
        /// Calls value change func when it is set
        Sync,

        /// Does not call value change func when it is set
        None
    };

    /// When the value changes, call a func
    template <class Type>
        requires std::equality_comparable<Type>
    struct ObservedValue {
    public:
        using This = ObservedValue;
        using OnValueChangeFunc = std::function<void(Type)>;

    protected:
        Type value{};

        OnValueChangeFunc onValueChangeFunc;

        void OnValueChange() {
            GUARD(onValueChangeFunc)
            onValueChangeFunc(value);
        }

    public:
        explicit ObservedValue(Type value = {}) :
            value(value) {}

        /// Sets the on value change func and optionally syncs it with the current value
        void SetOnValueChangeFunc(
            SetOnValueChangeFuncType setOnValueChangeFuncType, OnValueChangeFunc value
        ) {
            onValueChangeFunc = value;

            switch (setOnValueChangeFuncType) {
            case SetOnValueChangeFuncType::Sync:
                OnValueChange();
                break;
            default:
                break;
            }
        }

        Type const& Value() const {
            return value;
        }

        void SetValue(Type const& value) {
            GUARD(this->value != value)
            this->value = value;

            OnValueChange();
        }

        void SetValue(Type&& value) {
            GUARD(this->value != value)
            this->value = std::move(value);

            OnValueChange();
        }

        operator Type() const {
            return value;
        }

        This& operator=(Type const& value) {
            SetValue(value);
            return *this;
        }
    };
} // namespace PJ
