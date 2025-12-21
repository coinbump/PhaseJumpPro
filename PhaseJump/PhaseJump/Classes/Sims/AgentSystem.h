#pragma once

#include "AgentGroup.h"
#include "Dev.h"
#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    /// A simulation contains agents, each agent is responsible for its own localized logic
    /// For convenience, agents can be divided into Agent Groups
    class AgentSystem : public SomeUpdatable {
    public:
        using GroupList = VectorList<SP<AgentGroup>>;

    protected:
        GroupList groups;
        Updatable updatable;

    public:
        AgentSystem() {
            updatable.onUpdateFunc = [this](auto& updatable, auto time) {
                for (auto& group : groups) {
                    group->OnUpdate(time);
                }

                return FinishType::Continue;
            };
        }

        GroupList const& Groups() const {
            return groups;
        }

        void Add(SP<AgentGroup> group) {
            GUARD(group)
            GUARD_LOG(group->system == nullptr, "ERROR. Can't add owned sim group")

            groups.push_back(group);
            group->system = this;
        }

        // MARK: SomeUpdatable

        FinishType OnUpdate(TimeSlice time) override {
            return updatable.OnUpdate(time);
        }

        bool IsFinished() const override {
            return updatable.IsFinished();
        }
    };
} // namespace PJ
