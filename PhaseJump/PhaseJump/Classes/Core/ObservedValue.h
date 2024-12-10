#pragma once

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/30/24
 */
namespace PJ {
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

    public:
        explicit ObservedValue(Type value = {}) :
            value(value) {}

        void SetOnValueChangeFunc(OnValueChangeFunc value) {
            onValueChangeFunc = value;
            OnValueChange();
        }

        Type Value() const {
            return value;
        }

        void SetValue(Type const& value) {
            GUARD(this->value != value)
            this->value = value;

            OnValueChange();
        }

        void OnValueChange() {
            GUARD(onValueChangeFunc)
            onValueChangeFunc(value);
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
