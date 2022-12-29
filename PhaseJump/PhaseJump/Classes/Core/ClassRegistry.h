#ifndef PJCLASSREGISTRY_H
#define PJCLASSREGISTRY_H

#include "_String.h"
#include <map>
#include "Factory.h"
#include "Collections/_Map.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/10/22
 */
namespace PJ
{
    /// <summary>
    /// Registry for classes
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    class ClassRegistry : public Map<String, std::shared_ptr<Class>>
    {
    public:
        ClassRegistry() {
        }

        template <class T> std::shared_ptr<T> NewType(String key) {
            auto iterator = this->find(key);
            if (iterator == this->end()) { return NULL; }

            auto ptr = iterator->second;
            auto value = ptr.get();
            auto typeClass = dynamic_cast<TypeClass<T>*>(value);
            if (nullptr == typeClass) { return nullptr; }
            if (nullptr == typeClass->factory.get()) { return nullptr; }

            return typeClass->factory->New();
        }

        std::shared_ptr<Base> New(String key) const {
            auto iterator = this->find(key);
            if (iterator == this->end()) { return NULL; }

            auto ptr = iterator->second;
            auto value = ptr.get();

            return value->New();
        }
    };
}

#endif
