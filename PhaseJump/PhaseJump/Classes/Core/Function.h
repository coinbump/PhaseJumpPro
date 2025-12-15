#pragma once

#include "List.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/14/24
 */
namespace PJ {
    template <typename Result, typename... Arguments>
    class SomeFunction;

    /// Wraps std::function so we can subclass and add state if needed
    template <typename Result, typename... Arguments>
    class SomeFunction<Result(Arguments...)> {
    public:
        using Func = std::function<Result(Arguments... args)>;

        virtual ~SomeFunction() {}

        virtual Result operator()(Arguments... args) = 0;
    };

    template <typename Result, typename... Arguments>
    class Function;

    /// Wraps std::function so we can subclass and add state if needed
    template <typename Result, typename... Arguments>
    class Function<Result(Arguments...)> : public SomeFunction<Result(Arguments...)> {
    public:
        using Func = std::function<Result(Arguments... args)>;

        Func func;

        Function(Func func) :
            func(func) {}

        // MARK: SomeFunction

        Result operator()(Arguments... args) override {
            GUARDR(func, Result())
            return func(args...);
        }
    };

    // TODO: can this just be a mutable lambda?
    /// A function that stores persistent state
    template <typename Core, typename Result, typename... Arguments>
    class CoreFunction : public SomeFunction<Result(Arguments...)> {
    public:
        using CoreFunc = std::function<Result(Core& core, Arguments... args)>;

        Core core{};
        CoreFunc coreFunc;

        CoreFunction() {}

        CoreFunction(CoreFunc coreFunc) :
            coreFunc(coreFunc) {}

        // MARK: SomeFunction

        Result operator()(Arguments... args) override {
            GUARDR(coreFunc, Result())
            return coreFunc(core, args...);
        }
    };

} // namespace PJ
