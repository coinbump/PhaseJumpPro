#include "UIWorldSystem.h"
#include "Dev.h"
#include "HoverGestureHandler.h"
#include "World.h"

using namespace std;
using namespace PJ;

Camera* UIWorldSystem::GetCamera() const {
    auto world = World();
    GUARDR(world, {})

    return world->MainCamera();
}

FinishType UIWorldSystem::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    OnDragUpdate();

    return FinishType::Continue;
}

void UIWorldSystem::UpdateActiveHover(VectorList<PJ::LocalHit> hits) {
    auto hoverHits =
        Filter(hits, [](auto& hit) { return hit.node.IsListenerToSignal(SignalId::Hover); });

    GUARD(!IsEmpty(hoverHits))

    auto& node = hoverHits[0].node;
    SetActiveHover(SCAST<WorldNode>(node.shared_from_this()));
}

void UIWorldSystem::SetActiveHover(SP<WorldNode> value) {
    auto activeHover = ActiveHover();
    GUARD(activeHover != value)

    auto updateHover = [](WorldNode& node, bool isHovering) {
        HoverUIEvent event{ isHovering };
        node.Signal(SignalId::Hover, event);
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

void UIWorldSystem::OnPointerMove(PointerMoveUIEvent const& event) {
    Base::OnPointerMove(event);

    if (IsDragging()) {
        CheckDropTargets(event.screenPos);
        return;
    }

    // This only supports a single object for enter/exit pointer
    // FUTURE: support multiple if needed
    auto hits = TestScreenHit(event.screenPos);
    auto hit = hits.size() > 0 ? &hits[0] : nullptr;
    if (!hit) {
        SetActiveHover(nullptr);

        // Nothing was hit, send pointer exit events
        for (auto& node : pointerEnterNodes) {
            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;

            PointerExitUIEvent event;
            DispatchEvent(*node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();
    } else {
        // We hit something, send pointer exit events for the others and pointer enter for this
        auto& hitNode = hit->node;

        UpdateActiveHover(hits);

        bool isHitInPointerEnterNodes = false;
        for (auto& node : pointerEnterNodes) {
            if (node.get() == &hitNode) {
                isHitInPointerEnterNodes = true;
                continue;
            }

            // cout << "Log: Pointer Exit: " <<
            // pointerMotionEvent->position.ToString() << std::endl;
            PointerExitUIEvent event;
            DispatchEvent(*node, SignalId::PointerExit, event);
        }
        pointerEnterNodes.clear();

        pointerEnterNodes.insert(SCAST<WorldNode>(hitNode.shared_from_this()));

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

    auto hits = TestScreenHit(pointerDownEvent.core.screenPos);
    for (auto& hit : hits) {
        // FUTURE: only send pointer event to the topmost hit that also supports pointer down
        // signals cout << "Log: HIT: OnPointerDown\n";

        // Send mouse down event to node components
        auto& hitNode = hit.node;
        DispatchEvent(hitNode, SignalId::PointerDown, pointerDownEvent);

        pointerDownNodesMap[pointerDownEvent.core.button].clear();
        pointerDownNodesMap[pointerDownEvent.core.button].insert(
            SCAST<WorldNode>(hitNode.shared_from_this())
        );
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

    auto iterNodes = pointerDownNodesMap[pointerUpEvent.core.button];
    for (auto& node : iterNodes) {
        DispatchEvent(*node, SignalId::PointerUp, pointerUpEvent);
    }
}
