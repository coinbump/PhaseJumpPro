#pragma once

#include "Base.h"
#include "List.h"
#include <algorithm>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// An operation is a runnable piece of code that can be queued, along with other operations
    class SomeOperation : public Base {
    public:
        virtual void Run() = 0;

        void operator()() {
            Run();
        }
    };

    /// Operation with a core and run func
    template <class Core = Void>
    class Operation : public SomeOperation {
    public:
        using Base = SomeOperation;
        using This = Operation;

        using OperationFunc = std::function<void(This&)>;

        Core core{};

        Operation(Core core = {}, OperationFunc runFunc = {}) :
            core(core),
            runFunc(runFunc) {}

        OperationFunc runFunc;

        // MARK: SomeOperation

        void Run() override {
            GUARD(runFunc)
            runFunc(*this);
        }

        // MARK: StringConvertible

        /// @return Returns description for logs
        String ToString() const override {
            return "Operation";
        }
    };

    /// Operation that accepts input and produces output
    template <class Input, class Output>
    class SomeResultOperation : public Base {
    public:
        virtual Output Run(Input args) = 0;
    };

    /// Result operation with a core and run func
    template <class Input, class Output, class Core = Void>
    class ResultOperation : public SomeResultOperation<Input, Output> {
    public:
        using Base = SomeResultOperation<Input, Output>;
        using This = ResultOperation<Input, Output, Core>;

        using OperationFunc = std::function<Output(This&, Input&)>;

        Core core{};

        struct Config {
            Core core{};
            OperationFunc runFunc;
        };

        ResultOperation(Config const& config) :
            core(config.core),
            runFunc(config.runFunc) {}

        OperationFunc runFunc;

        // MARK: SomeOperation

        Output Run(Input args) override {
            GUARDR(runFunc, {})
            return runFunc(*this, args);
        }
    };

    /// Operation queue
    class OperationQueue : public Base {
    public:
        VectorList<UP<SomeOperation>> operations;
    };
} // namespace PJ
