#pragma once

#include "Base.h"
#include "Class.h"
#include "Factory.h"
#include "FactoryRegistry.h"
#include "List.h"
#include "StringUtils.h"
#include "Tags.h"
#include "Utils.h"

// Experimental
namespace PJ {
    /// Allows us to define custom properties for an environment
    /// A new environment can be pushed, allowing us to modify specific environment values for the
    /// latest context That environment is then popped off the stack when that context is finished
    ///
    // push pop DOES NOT WORK, because we might push, then access variables later
    class Environment {
    public:
        /// Environment tag values
        Tags tags;

    public:
        using Key = Tags::Key;

        virtual ~Environment() {}

        template <class T>
        std::optional<T> Value(Key key) const {
            return tags.Value<T>(key);
        }

        template <class T>
        void SetValue(Key key, T value) {
            return tags.map[key] = value;
        }
    };

    template <class T>
    class SomeEnvironmentKey {
    public:
        virtual ~SomeEnvironmentKey() {}

        virtual T DefaultValue() const = 0;
    };

    class SomeEnvironmentValueClass : public Class<> {
    public:
        SomeEnvironmentValueClass(String id) :
            Class({ .id = id }) {}
    };

    /// Register each environment value in the Environments so we have access to defaultValue
    template <class T>
    struct EnvironmentValueClass : public SomeEnvironmentValueClass {
        T defaultValue;

        EnvironmentValueClass(String id, T defaultValue) :
            SomeEnvironmentValueClass(id),
            defaultValue(defaultValue) {}

        T Value(Environment& environment) const {
            auto optionalResult = environment.Value<T>();
            GUARDR(optionalResult, defaultValue)

            return optionalResult;
        }

        void SetValue(Environment& environment, T value) {
            environment.SetValue<T>(value);
        }
    };

    class Environments {
    protected:
        VectorList<SP<Environment>> stack;
        std::mutex stackMutex;

        static SP<Environments> shared;
        static std::mutex sharedMutex;

    public:
        UnorderedMap<String, SP<SomeEnvironmentValueClass>> valueClasses;

        static Environments& Shared() {
            if (!shared) {
                std::lock_guard guard(sharedMutex);
                shared = MAKE<Environments>();
            }
            return *shared;
        }

        void Register(SP<SomeEnvironmentValueClass> _class) {
            valueClasses[_class->id] = _class;
        }

        template <class T>
        T Value(String id) {
            auto currentEnvironment = Current();
            GUARDR(currentEnvironment, T())

            auto i = valueClasses.find(id);
            GUARD(i != valueClasses.end())

            auto typeValueClass = DCAST<EnvironmentValueClass<T>>(i->second);
            GUARDR(typeValueClass, T())

            return typeValueClass->Value(*currentEnvironment);
        }

        SP<Environment> Top() const {
            return stack.size() > 0 ? stack.back() : nullptr;
        }

        SP<Environment> Push() {
            std::lock_guard guard(stackMutex);
            SP<Environment> result = MAKE<Environment>();

            auto top = Top();
            if (top) {
                // Copy over environment values so they can be modified
                *result = *top;
            }

            Add(stack, result);

            return result;
        }

        SP<Environment> Pop() {
            GUARDR(!IsEmpty(stack), nullptr)

            std::lock_guard guard(stackMutex);
            auto result = stack.back();
            stack.pop_back();

            return result;
        }

        // Convenience
        SP<Environment> Current() const {
            return Top();
        }
    };

    struct EnvironmentPusher {
        SP<Environment> environment;

        EnvironmentPusher() :
            environment(Environments::Shared().Push()) {}

        ~EnvironmentPusher() {
            Environments::Shared().Pop();
        }
    };
} // namespace PJ
