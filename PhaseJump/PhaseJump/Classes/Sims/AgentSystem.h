#pragma once

#include "AgentGroup.h"
#include "Macros.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    /// A simulation contains agents, each agent is responsible for its own localized logic
    /// For convenience, agents can be divided into Agent Groups
    class AgentSystem : public Updatable {
    public:
        using GroupList = VectorList<SP<SomeAgentGroup>>;

    protected:
        GroupList groups;

    public:
        GroupList const& Groups() const {
            return groups;
        }

        void Add(SP<SomeAgentGroup> group) {
            GUARD(group)
            GUARD_LOG(group->system == nullptr, "ERROR. Can't add owned sim group")

            groups.push_back(group);
            group->system = this;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            for (auto& group : groups) {
                group->OnUpdate(time);
            }
        }
    };
} // namespace PJ
