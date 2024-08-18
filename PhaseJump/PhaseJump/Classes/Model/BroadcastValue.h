// #pragma once
//
// #include "Broadcaster.h"
// #include "Utils.h"
// #include "SomeEvent.h"
// #include "TransformFunc.h"
// #include <memory>
//
///*
// RATING: 5 stars
// Has unit tests
// CODE REVIEW: 7/5/24
// */
// namespace PJ {
//    template <class T>
//    class BroadcastValue;
//
//    /// Sent when a value changes
//    template <class T>
//    class ValueChangeEvent : public SomeEvent {
//    public:
//        using BroadcastValueSharedPtr = SP<BroadcastValue<T>>;
//        BroadcastValueSharedPtr value;
//
//        ValueChangeEvent() {}
//
//        ValueChangeEvent(BroadcastValueSharedPtr const& value) :
//            value(value) {}
//    };
//
//    /// Broadcasts when value changes.
//    template <class T>
//    class BroadcastValue : public std::enable_shared_from_this<BroadcastValue<T>> {
//    protected:
//        T value;
//
//        virtual void OnValueChange() {
//            auto sharedThis = this->shared_from_this();
//            broadcaster.Broadcast(MAKE<ValueChangeEvent<T>>(sharedThis));
//        }
//
//    public:
//        Broadcaster broadcaster;
//
//        virtual ~BroadcastValue() {}
//
//        virtual T const& Value() const {
//            return value;
//        }
//
//        virtual void SetValue(T value) {
//            auto newValue = value;
//            if (this->value == newValue) {
//                return;
//            }
//
//            this->value = newValue;
//            OnValueChange();
//        }
//
//        BroadcastValue() {}
//
//        BroadcastValue(T value) :
//            value(value) {}
//
//        void AddListener(Broadcaster::ListenerWeakPtr listener) {
//            broadcaster.AddListener(listener);
//        }
//
//        /// Convenience function for responding to a value change event
//        /// Checks if the value change belongs to object and performs the action
//        /// if it is
//        void OnValueChange(SPC<SomeEvent> theEvent, std::function<void(T)> action) {
//            GUARD(action)
//
//            auto eventValueChange = DCAST<ValueChangeEvent<T>>(theEvent);
//            if (nullptr != eventValueChange && eventValueChange->value.get() == this) {
//                action(value);
//            }
//        }
//    };
//
//    template <class T>
//    class BroadcastTransformValue : public BroadcastValue<T> {
//    public:
//        using Base = BroadcastValue<T>;
//
//        SP<SomeValueTransform<T>> transform = MAKE<IdentityTransform<T>>();
//
//        BroadcastTransformValue(T value) :
//            Base(value) {}
//
//        virtual void SetValue(T value) override {
//            Base::SetValue(transform->Transform(value));
//        }
//    };
//} // namespace PJ
