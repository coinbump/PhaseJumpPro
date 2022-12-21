#ifndef PJPUBLISHEDVALUE_H
#define PJPUBLISHEDVALUE_H

#include "Event.h"
#include "Broadcaster.h"
#include "SomeTransform.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22UnitTests
 */
namespace PJ
{
    template <class T> class PublishedValue;

    /// <summary>
    /// Sent when a value changes
    /// </summary>
    template <class T>
    class EventPublishedChange : public Event
    {
    public:
        using PublishedValueSharedPtr = std::shared_ptr<PublishedValue<T>>;
        PublishedValueSharedPtr value;

        EventPublishedChange()
        {
        }

        EventPublishedChange(PublishedValueSharedPtr const& value) : value(value)
        {
        }
    };

    /// <summary>
    /// Used for type erasure to store Published value in a collection
    /// </summary>
    class AnyPublished
    {
    };

    template <class T>
    class SomePublished : public AnyPublished
    {
    public:
        virtual T Value() const = 0;
        virtual void SetValue(T value) = 0;

        virtual void AddListener(Broadcaster::ListenerWeakPtr listener) = 0;

        virtual ~SomePublished() {}
    };

    /// <summary>
    /// Broadcasts when value changes.
    /// </summary>
    template <class T>
    class PublishedValue : public SomePublished<T>, public std::enable_shared_from_this<PublishedValue<T>>
    {
    protected:
        T value;

        virtual void OnValueChange()
        {
            auto sharedThis = this->shared_from_this();
            broadcaster->Broadcast(std::make_shared<EventPublishedChange<T>>(sharedThis));
        }

    public:
        std::shared_ptr<Broadcaster> broadcaster = std::make_shared<Broadcaster>();
        std::shared_ptr<SomeValueTransform<T>> transform = std::make_shared<IdentityTransform<T>>();

        virtual T Value() const { return value; }
        virtual void SetValue(T value)
        {
            auto newValue = transform->Transform(value);
            if (this->value == newValue) { return; }

            this->value = newValue;
            OnValueChange();
        }

        PublishedValue()
        {
        }

        PublishedValue(T value) : value(value)
        {
        }

        void AddListener(Broadcaster::ListenerWeakPtr listener)
        {
            broadcaster->AddListener(listener);
        }

        /// <summary>
        /// Convenience function for responding to a value change event
        /// Checks if the value change belongs to this->Published object and performs the action if it is
        /// </summary>
        void OnValueChange(Broadcaster::EventSharedPtr theEvent, std::function<T> action)
        {
            auto eventValueChange = std::dynamic_pointer_cast<EventPublishedChange<T>>(theEvent);
            if (nullptr != eventValueChange && eventValueChange.value == this)
            {
                action(value);
            }
        }
    };
}

#endif
