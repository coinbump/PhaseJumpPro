//
//  PublishedValueScrap.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 12/11/22.
//

#ifndef PublishedValueScrap_h
#define PublishedValueScrap_h


/// <summary>
/// Broadcasts when value changes, with Equatable requirement.
/// </summary>
//    class PublishedValue<T> : PublishedEntity<T> where T : IEquatable<T>
//    {
//        override T Value
//        {
//            get => value;
//            set
//            {
//                auto newValue = transform.Transform(value);
//                auto isNullToValueChange = (this->value == null && newValue != null) || (this->value != null && newValue == null);
//
//                if (isNullToValueChange || !newValue.Equals(this->value))
//                {
//                    this->value = newValue;
//                    OnValueChange();
//                }
//            }
//        }
//
//        /// <summary>
//        /// Conversion operator
//        /// </summary>
//        static explicit operator T(PublishedValue<T> value)
//        {
//            return value.Value;
//        }
//
//        PublishedValue()
//        {
//        }
//
//        PublishedValue(T value)
//        {
//            this->value = value;
//        }
//
//        bool Equals(T value) const
//        {
//            auto myValue = Value;
//            if (null == myValue && null == value) { return true; }
//            if (null == myValue || null == value) { return false; }
//
//            return myValue.Equals(value);
//        }
//    };

#endif /* PublishedValueScrap_h */
