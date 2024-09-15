#pragma once

#include "Base.h"
#include "StringUtils.h"
#include "UnorderedMap.h"
#include "Utils.h"
#include <mutex>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// Registered in a dependency container to produce an object
    template <class Type>
    class SomeDependency : public Base {
    public:
        virtual SP<Type> Resolve() = 0;
    };

    /// Stores the cached dependency after it is created
    template <class T>
    class CachedDependency : public SomeDependency<T> {
    public:
        using Type = T;
        using Value = SP<Type>;

    protected:
        std::mutex valueMutex;
        std::function<Value()> resolver;
        Value value;

    public:
        CachedDependency(std::function<Value()> resolver) :
            resolver(resolver) {}

        Value Resolve() override {
            GUARDR(!value, value);
            GUARDR(resolver, nullptr)

            std::lock_guard guard(valueMutex);
            value = resolver();

            return value;
        }

        void Flush() {
            std::lock_guard guard(valueMutex);
            value.reset();
        }

        bool IsCached() const {
            return value != nullptr;
        }
    };

    /// Contains N registered dependencies that can be resolved to retrieve an object
    class DependencyContainer {
    protected:
        UnorderedMap<String, SP<Base>> dependencies;
        std::mutex dependenciesMutex;

    public:
        template <class Dependency>
        void Register(SP<Dependency> dependency) {
            std::lock_guard guard(dependenciesMutex);
            dependencies[typeid(typename Dependency::Type).name()] = dependency;
        }

        template <class Type>
        SomeDependency<Type>* Dependency() {
            auto i = dependencies.find(typeid(Type).name());
            GUARDR(i != dependencies.end(), nullptr)

            auto typeDependency = As<SomeDependency<Type>>(i->second.get());
            GUARDR(typeDependency, nullptr)

            return typeDependency;
        }

        template <class Type>
        SP<Type> Resolve() {
            auto typeDependency = Dependency<Type>();
            GUARDR(typeDependency, nullptr)

            return typeDependency->Resolve();
        }

        size_t Count() const {
            return dependencies.size();
        }
    };
} // namespace PJ
