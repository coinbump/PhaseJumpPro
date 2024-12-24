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

        using OperationFunc = std::function<void(Operation&)>;

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
    };

    /// Operation queue
    class OperationQueue : public Base {
    public:
        VectorList<SP<SomeOperation>> operations;
    };
} // namespace PJ
