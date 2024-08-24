#pragma once

#include "Function.h"
#include "List.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/10/24
 */
namespace PJ {
    template <typename Result, typename... Arguments>
    class MultiFunction;

    /// Composes multiple ordered functions
    /// Each result is combined into the final result by a reducer
    /// Default reducer only returns the final result
    template <typename Result, typename... Arguments>
    class MultiFunction<Result(Arguments...)> : public SomeFunction<Result(Arguments...)> {
    public:
        using Func = std::function<Result(Arguments... args)>;
        using Reducer = std::function<Result(Result const&, Result)>;

        List<Func> funcs;
        std::optional<Reducer> reducer;

        MultiFunction() {}

        Result Run(Arguments... args) const {
            std::optional<Result> result;

            auto iterFuncs = funcs;
            std::for_each(iterFuncs.begin(), iterFuncs.end(), [&](Func& func) {
                GUARD(func)
                Result thisResult = func(args...);

                if (reducer && result) {
                    Result reducedResult = reducer.value()(*result, thisResult);
                    result = reducedResult;
                } else {
                    result = thisResult;
                }
            });
            return result.value_or(Result());
        }

        // MARK: SomeFunction

        Result operator()(Arguments... args) override {
            return Run(args...);
        }

        // MARK: Convenience

        void Add(Func value) {
            this->funcs.push_back(value);
        }
    };

    /// Allows us to compose multiple ordered functions instead of using inheritance
    /// Each result is combined into the final result by a reducer
    /// Default reducer only returns the final result
    template <typename... Arguments>
    class MultiFunction<void(Arguments...)> {
    public:
        using Func = std::function<void(Arguments... args)>;

        List<Func> funcs;

        MultiFunction() {}

        void Run(Arguments... args) const {
            auto iterFuncs = funcs;
            std::for_each(iterFuncs.begin(), iterFuncs.end(), [&](Func& func) {
                GUARD(func)
                func(args...);
            });
        }

        void operator()(Arguments... args) {
            return Run(args...);
        }

        // MARK: Convenience

        void Add(Func value) {
            this->funcs.push_back(value);
        }
    };
} // namespace PJ
