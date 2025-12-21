#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    class Void;

    /// Wraps a transform func so we can optionally store state
    template <class Input, class Output = Input>
    class SomeTransformer {
    public:
        virtual ~SomeTransformer() {}

        virtual Output Transform(Input value) = 0;

        Output operator()(Input value) {
            return Transform(value);
        }
    };

    /// Wraps a transform func with state
    template <class Core, class Input, class Output = Input>
    class Transformer : public SomeTransformer<Input, Output> {
    public:
        using Base = SomeTransformer<Input, Output>;
        using This = Transformer<Core, Input, Output>;

        using TransformFunc = std::function<Output(This&, Input)>;

        Core core{};

        TransformFunc func;

        Transformer(Core const& core, TransformFunc func) :
            func(func),
            core(core) {}

        Output Transform(Input value) override {
            GUARDR(func, {})
            return func(*this, value);
        }
    };
} // namespace PJ
