#ifndef PJEVENTSYSTEM_H
#define PJEVENTSYSTEM_H

#include "Camera.h"
#include "VectorList.h"
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

    /// Handles input
    class EventSystem : public WorldSystem {
        void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) override;
    };
}

#endif
