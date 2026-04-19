#include "CharacterController.h"
#include "DelayBehavior.h"
#include "ParallelBehavior.h"
#include "RepeatBehavior.h"
#include "SequenceBehavior.h"
#include "TimedBehavior.h"
#include "Timeline.h"
#include "UIPlanner.h"
#include "UpdatableBehavior.h"

using namespace std;
using namespace PJ;

std::function<void(UIPlanner&, BehaviorNode*)> planBehaviorUIFunc = [](UIPlanner& planner,
                                                                       BehaviorNode* node) {
    GUARD(node->IsRunning())

    std::stringstream ss;
    ss << "Behavior: " + node->name;

    planner.TreeNode(
        { .label = ss.str(),
          .isOpenDefault = true,
          .planUIFunc =
              [=](auto& planner) {
                  auto repeatBehavior = dynamic_cast<RepeatBehavior*>(node);
                  if (repeatBehavior) {
                      planner.LabelText({ .text = "Repeat Max: " +
                                                  MakeString(repeatBehavior->RepeatMax()) });
                      planner.LabelText({ .text = "Repeat Count: " +
                                                  MakeString(repeatBehavior->RepeatCount()) });
                  }

                  auto delayBehavior = dynamic_cast<DelayBehavior*>(node);
                  if (delayBehavior) {
                      planner.LabelText({ .text = "Delay Max: " +
                                                  MakeString(delayBehavior->DelayMax()) });
                      planner.LabelText({ .text = "Delay Remain: " +
                                                  MakeString(delayBehavior->DelayRemainder()) });
                  }

                  auto timedBehavior = dynamic_cast<TimedBehavior*>(node);
                  if (timedBehavior) {
                      planner.LabelText({ .text = "Time Max: " +
                                                  MakeString(timedBehavior->TimeMax()) });
                      planner.LabelText({ .text = "Time Remain: " +
                                                  MakeString(timedBehavior->TimeRemainder()) });
                  }

                  auto parallelBehavior = dynamic_cast<ParallelBehavior*>(node);
                  if (parallelBehavior) {
                      planner.LabelText({ .text =
                                              "Success Count: " +
                                              MakeString(parallelBehavior->ChildSuccessCount()) });
                      planner.LabelText({ .text =
                                              "Failure Count: " +
                                              MakeString(parallelBehavior->ChildFailureCount()) });
                  }

                  auto sequenceBehavior = dynamic_cast<SequenceBehavior*>(node);
                  if (sequenceBehavior) {
                      auto target = sequenceBehavior->Target();
                      if (target) {
                          planner.LabelText({ .text = "Target: " + target->name });
                      }
                  }

                  auto updatableBehavior = dynamic_cast<UpdatableBehavior*>(node);
                  if (updatableBehavior) {
                      auto timeline =
                          dynamic_cast<Timeline*>(updatableBehavior->GetChildUpdatable());
                      if (timeline) {
                          planner.LabelText({ .text = "Duration: " +
                                                      MakeString(timeline->Duration()) });
                          planner.LabelText({ .text = "Play Time: " +
                                                      MakeString(timeline->PlayTime()) });
                          planner.LabelText({ .text = "Progress: " +
                                                      MakeString(timeline->Progress()) });

                          for (auto& track : timeline->Tracks()) {
                              planner.LabelText({ .text = "Track: " + MakeString(track->id) });
                              planner.LabelText({ .text = "  Track Time: " +
                                                          MakeString(track->PlayTime()) });

                              auto floatTimeTrack = dynamic_cast<TimeTrack<float>*>(track.get());
                              if (floatTimeTrack) {
                                  auto keyframe =
                                      floatTimeTrack->KeyframeAtOrBefore(track->PlayTime());
                                  if (keyframe) {
                                      planner.LabelText({ .text = "    Keyframe: " +
                                                                  MakeString(keyframe->time) });
                                  }
                              }
                          }
                      }
                  }

                  for (auto& node : node->Children()) {
                      planBehaviorUIFunc(planner, node.get());
                  }
              } }
    );
};

CharacterController::CharacterController() {
    StateMachine<StateType>::OnStateChangeFunc onStateChangeFunc = [this](auto& states) {
        behavior.reset();

        try {
            auto state = states.State();
            auto& buildFunc = buildBehaviorFuncs.at(state);
            behavior = buildFunc(*this);
        } catch (...) {}
    };

    states.OverrideOnStateChangeFunc(onStateChangeFunc);

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner.LabelText({ .label = "State", .text = states.State() });

        GUARD(behavior)
        planBehaviorUIFunc(planner, behavior.get());
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

FinishType CharacterController::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARDR(behavior, FinishType::Continue)
    return behavior->OnUpdate(time);
}

void CharacterController::AddBuildBehaviorFunc(StateType state, BuildBehaviorFunc func) {
    states.Add(state);
    buildBehaviorFuncs.insert_or_assign(state, func);

    GUARD(states.State() == state)
    states.ForceOnStateChange();
}
