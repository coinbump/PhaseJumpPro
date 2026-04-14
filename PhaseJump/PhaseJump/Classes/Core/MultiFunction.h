#pragma once

#include "Function.h"
#include "UnorderedMap.h"
#include "Utils.h"
#include "VectorList.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    template <typename Result, typename... Arguments>
    class MultiFunction;

    /// Creates string identifiers in an incrementing fashion ("0", "1", ...)
    class IncrementingIdentifier {
    protected:
        uint32_t id{};

    public:
        String Next() {
            auto oldId = id;
            id++;
            return MakeString(oldId);
        }
    };

    /// Composes multiple ordered functions
    /// Each result is combined into the final result by a reducer
    /// Default reducer only returns the final result
    /// Similar to the Delegate type in C#
    template <typename Result, typename... Arguments>
    class MultiFunction<Result(Arguments...)> final : public SomeFunction<Result(Arguments...)> {
    public:
        using Func = std::function<Result(Arguments... args)>;
        using Reducer = std::function<Result(Result const&, Result)>;

    protected:
        UnorderedMap<String, Func> funcs;

    public:
        std::optional<Reducer> reducer;

        MultiFunction() {}

        Result Run(Arguments... args) const {
            std::optional<Result> result;

            auto iterFuncs = funcs;
            std::for_each(iterFuncs.begin(), iterFuncs.end(), [&](auto& pair) {
                GUARD(pair.second)
                Result thisResult = pair.second(args...);

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

        void Add(String id, Func value) {
            funcs[id] = value;
        }

        void Remove(String id) {
            funcs.erase(id);
        }
    };

    /// Combines multiple ordered functions with identical arguments
    /// Similar to the Delegate type in C#
    template <typename... Arguments>
    class MultiFunction<void(Arguments...)> {
    public:
        using Func = std::function<void(Arguments... args)>;

    protected:
        UnorderedMap<String, Func> funcs;

    public:
        MultiFunction() {}

        void Run(Arguments... args) const {
            auto iterFuncs = funcs;
            std::for_each(iterFuncs.begin(), iterFuncs.end(), [&](auto& pair) {
                GUARD(pair.second)
                pair.second(args...);
            });
        }

        void operator()(Arguments... args) {
            Run(args...);
        }

        void Add(String id, Func value) {
            funcs[id] = value;
        }

        void Remove(String id) {
            funcs.erase(id);
        }
    };
} // namespace PJ
