#ifndef PJSOMEPERATION_H
#define PJSOMEPERATION_H

#include "Base.h"
#include "List.h"
#include <algorithm>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Operations are added to a queue
    class SomeOperation : public Base {
    public:
        using RootBaseType = Base;

        virtual void Run() = 0;

        void operator()() {
            Run();
        }
    };

    class OperationQueue : public Base {

        List<SP<SomeOperation>> operations;
    };
} // namespace PJ

#endif
