#ifndef PJSOMEPERATION_H
#define PJSOMEPERATION_H

#include "Base.h"
#include "VectorList.h"

/*
 RATING: 4 stars
 Simple interface, could use more logic
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    /// Operations are added to a queue
    class SomeOperation : public Base {
        virtual void Run() = 0;
    };

    class OperationQueue : public Base {
        VectorList<SP<SomeOperation>> operations;
    };
}

#endif
