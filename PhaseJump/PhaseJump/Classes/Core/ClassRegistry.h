#ifndef PJCLASSREGISTRY_H
#define PJCLASSREGISTRY_H

#include <string>
#include <map>
#include "Factory.h"
#include "_Map.h"

/*
 RATING: 5 stars
 Simple wrapper class with unit tests
 CODE REVIEW: 11/10/22
 */
namespace PJ
{
    /// <summary>
    /// Registry for classes
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    class ClassRegistry : public Map<std::string, std::shared_ptr<Class>>
    {
    public:
        ClassRegistry() {
        }

        template <class T> T* NewType(std::string key) {
            auto iterator = this->find(key);
            if (iterator == this->end()) { return NULL; }

            auto ptr = iterator->second;
            auto value = ptr.get();
            auto typeClass = dynamic_cast<TypeClass<T>*>(value);
            if (NULL == typeClass) { return NULL; }
            if (NULL == typeClass->factory.get()) { return NULL; }

            return typeClass->factory->New();
        }

        Base* New(std::string key) const {
            auto iterator = this->find(key);
            if (iterator == this->end()) { return NULL; }

            auto ptr = iterator->second;
            auto value = ptr.get();

            return value->New();
        }
    };
}

#endif
