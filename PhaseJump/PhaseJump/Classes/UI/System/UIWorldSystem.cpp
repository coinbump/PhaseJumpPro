#include "UIWorldSystem.h"
#include "Dev.h"
#include "SomeHoverGestureHandler.h"
#include "World.h"

using namespace std;
using namespace PJ;

UIWorldSystem* UIWorldSystem::shared;

SP<SomeCamera> UIWorldSystem::Camera() const {
    return World()->MainCamera();
}

void UIWorldSystem::ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {
    Base::ProcessUIEvents(uiEvents);
}

void UIWorldSystem::Awake() {
    Base::Awake();

    if (nullptr != shared) {
        PJ::Log("ERROR. Only 1 UISystem supported");
        return;
    }
    shared = this;
}

void UIWorldSystem::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    OnDragUpdate();
}

void UIWorldSystem::UpdateActiveHover(VectorList<PJ::LocalHit> hits) {
    for (auto& hit : hits) {
        for (auto& component : hit.node->Components()) {
            try {
                auto handler = component->signalHandlers.at(SignalId::Hover);
                SetActiveHover(hit.node);
                return;
            } catch (...) {}
        }
    }
}

void UIWorldSystem::SetActiveHover(SP<WorldNode> value) {
    auto activeHover = ActiveHover();
    GUARD(activeHover != value)

    auto updateHover = [](WorldNode& node, bool isHovering) {
        for (auto& component : node.Components()) {
            try {
                auto handler = component->signalHandlers.at(SignalId::Hover);
                HoverUIEvent event{ isHovering };
                handler(*component, event);
            } catch (...) {}
        }
    };

    if (activeHover) {
        updateHover(*activeHover, false);
    }

    this->activeHover.reset();

    if (value) {
        this->activeHover = value;
        updateHover(*value, true);
    }
}

SP<WorldNode> UIWorldSystem::ActiveHover() {
    return activeHover.lock();
}

void UIWorldSystem::OnMouseMotion(PointerMoveUIEvent const& event) {
    Base::OnMouseMotion(event);

    if (IsDragging()) {
        CheckDropTargets(event.screenPos);
        return;
    }

    // This only supports a single object for enter/exit pointer
    // FUTURE: support multiple if needed
    auto hits = TestLocalHit(event.screenPos);
    auto hit = hits.size() > 0 ? &hits[0] : nullptr;
    if (!hit) {
        SetActiveHover(nullptr);

        // Nothing was hit, send pointer exit events
        for (auto& node : pointerEnterNodes) {
            GUARD_CONTINUE(!node.expired())

            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;

            auto _node = node.lock();
            PointerExitUIEvent event;
            DispatchEvent(_node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();
    } else {
        // We hit something, send pointer exit events for the others and pointer enter for this
        auto hitNode = hit->node;

        UpdateActiveHover(hits);

        bool isHitInPointerEnterNodes = false;
        for (auto& node : pointerEnterNodes) {
            if (node.expired()) {
                continue;
            }

            auto _node = node.lock();
            if (_node == hitNode) {
                isHitInPointerEnterNodes = true;
                continue;
            }

            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;
            PointerExitUIEvent event;
            DispatchEvent(_node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();

        Add(pointerEnterNodes, hitNode);

        // The hit node already received a pointer enter event
        if (!isHitInPointerEnterNodes) {
            PointerEnterUIEvent event;
            DispatchEvent(hitNode, SignalId::PointerEnter, event);
        }
    }
}

void UIWorldSystem::OnPointerDown(PointerDownUIEvent const& pointerDownEvent) {
    Base::OnPointerDown(pointerDownEvent);

    if (lockDrag) {
        switch (dragState) {
        case DragState::LockDragMouseDown:
            break;
        case DragState::LockDragMouseUp:
            OnDragEnd();
            return;
        default:
            break;
        }
    }

    // cout << "Log: OnPointerDown\n";

    auto screenPos = pointerDownEvent.screenPos;
    auto hits = TestLocalHit(pointerDownEvent.screenPos);
    for (auto& hit : hits) {
        // TODO: only send pointer event to the topmost hit that also supports pointer down signals
        // cout << "Log: HIT: OnPointerDown\n";

        // Send mouse down event to node components
        // TODO: we aren't converting to local pos here
        auto localPointerDownEvent = PointerDownUIEvent(screenPos, pointerDownEvent.button);
        auto hitNode = hit.node;
        DispatchEvent(hitNode, SignalId::PointerDown, localPointerDownEvent);

        pointerDownNodesMap[pointerDownEvent.button].clear();
        Add(pointerDownNodesMap[pointerDownEvent.button], hitNode);
    }
}

void UIWorldSystem::OnPointerUp(PointerUpUIEvent const& pointerUpEvent) {
    Base::OnPointerUp(pointerUpEvent);

    if (lockDrag) {
        switch (dragState) {
        case DragState::LockDragMouseDown:
            dragState = DragState::LockDragMouseUp;
            return;
        case DragState::LockDragMouseUp:
            break;
        default:
            break;
        }
    }

    auto iterNodes = pointerDownNodesMap[pointerUpEvent.button];
    for (auto& node : iterNodes) {
        auto _node = node.lock();
        GUARD_CONTINUE(_node)

        // Send event to node components
        // TODO: we aren't converting to local pos here
        auto localPointerUpEvent = PointerUpUIEvent(pointerUpEvent.button);
        DispatchEvent(_node, SignalId::PointerUp, localPointerUpEvent);
    }
}
