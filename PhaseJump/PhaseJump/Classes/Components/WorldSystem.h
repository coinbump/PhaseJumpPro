#ifndef PJWORLDSYSTEM_H
#define PJWORLDSYSTEM_H

#include "WorldComponent.h"
#include <memory>

namespace PJ {
    class SomeUIEvent;

    /// A component that receives system-level world events
    class WorldSystem : public WorldComponent {
    public:
        virtual void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) {
        }
    };
}

#endif
