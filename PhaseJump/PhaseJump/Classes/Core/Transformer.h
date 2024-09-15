#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/3/24
 PORTED TO: C++, C#
 */
namespace PJ {
    class Void;

    // TODO: once C# is cleaned up, can this be `Transform`?
    /// Wraps a transform func so we can optionally store state
    template <class Value, class Result = Value>
    class SomeTransformer {
    public:
        virtual ~SomeTransformer() {}

        virtual Result Transform(Value value) = 0;

        Result operator()(Value value) {
            return Transform(value);
        }
    };

    /// Wraps a transform func
    template <class Value, class Result = Value>
    class Transformer : public SomeTransformer<Value, Result> {
    public:
        using Func = std::function<Result(Value value)>;

        Func func;

        Transformer(Func func) :
            func(func) {}

        Result Transform(Value value) override {
            GUARDR(func, Result())
            return func(value);
        }
    };

    /// Wraps a transform func with state
    template <class Core, class Value, class Result = Value>
    class CoreTransformer : public SomeTransformer<Value, Result> {
    public:
        using This = CoreTransformer<Value, Result, Core>;
        using Func = std::function<Result(Core& core, Value value)>;

        Core core{};

        Func func;

        CoreTransformer(Func func, Core core = Core()) :
            func(func),
            core(core) {}

        Result Transform(Value value) override {
            GUARDR(func, Result())
            return func(core, value);
        }
    };
} // namespace PJ
