#ifndef PJEVENTSYSTEM_H
#define PJEVENTSYSTEM_H

#include "Camera.h"
#include "VectorList.h"
#include "WorldSystem.h"
#include "SomeUIEvent.h"
#include "_Map.h"
#include <memory>

namespace PJ {
    class WorldNode;

    /// Handles input events
    class EventSystem : public WorldSystem {
    protected:
        Map<PointerInputButton, VectorList<std::weak_ptr<WorldNode>>> pointerDownNodesMap;
        VectorList<std::weak_ptr<WorldNode>> pointerEnterNodes;

    public:
        void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) override;

    protected:
        void DispatchEvent(SPC<WorldNode> node, std::function<void(SP<SomePointerEventsResponder>)> command);

        virtual bool ProcessPointerDownEvent(SP<PointerDownUIEvent<ScreenPosition>> pointerDownEvent);
        virtual bool ProcessPointerUpEvent(SP<PointerUpUIEvent> pointerUpEvent);

        std::optional<LocalPosition> LocalHitPosition(ScreenPosition screenPosition);
    };
}

#endif
