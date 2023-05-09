#ifndef PJUISYSTEM_H
#define PJUISYSTEM_H

#include "WorldSystem.h"
#include <memory>

namespace PJ {
    class SomeUIEvent;

    /// Handles input
    class UISystem : public WorldSystem {
        void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) override;
    };
}

#endif
