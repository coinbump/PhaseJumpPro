#include "ParallelBehavior.h"

using namespace std;
using namespace PJ;

ParallelBehavior::ParallelBehavior(int successThreshold, int failureThreshold) :
    successThreshold(successThreshold),
    failureThreshold(failureThreshold) {}

VectorList<BehaviorNode*> ParallelBehavior::Targets() {
    VectorList<BehaviorNode*> result =
        Map<BehaviorNode*>(tree.Children(), [](auto& behavior) { return behavior.get(); });
    return result;
}

void ParallelBehavior::OnTargetFinish(BehaviorNode& target) {
    Base::OnTargetFinish(target);

    int successThreshold =
        this->successThreshold == AllThreshold ? (int)tree.ChildCount() : this->successThreshold;
    int failureThreshold =
        this->failureThreshold == AllThreshold ? (int)tree.ChildCount() : this->failureThreshold;

    int childSuccessCount = ChildSuccessCount();
    int childFailureCount = ChildFailureCount();

    if (childSuccessCount >= successThreshold) {
        finishState = BehaviorState::Success;
        Finish();
    } else if (childFailureCount >= failureThreshold) {
        finishState = BehaviorState::Failure;
        Finish();
    }
}
