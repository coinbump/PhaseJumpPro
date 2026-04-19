#include "Desktop.h"
#include "Window.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

Desktop::Desktop() {}

VectorList<SP<Window>> Desktop::Windows() const {
    VectorList<SP<Window>> result;
    GUARDR(owner, result)

    for (auto& childNode : owner->ChildNodes()) {
        GUARD_CONTINUE(childNode)

        auto window = childNode->TypeComponentPtr<Window>();
        GUARD_CONTINUE(window)

        result.push_back(window);
    }

    return result;
}

SP<Window> Desktop::NewWindow(WindowConfig const& config) {
    GUARDR(owner, nullptr)

    auto node = MAKE<WorldNode>(WorldNode::Config{ .name = "Window" });
    owner->Add(node);

    auto window = node->AddComponentPtr<Window>();
    window->buildContentFunc = config.buildContentFunc;
    window->worldSize = config.worldSize;
    window->closeFunc =
        config.closeFunc ? config.closeFunc : [this](Window& window) { Close(window); };

    if (config.windowLevel) {
        window->SetLevelZ(config.windowLevel->Z());
    }
    if (config.windowStyle) {
        window->model = config.windowStyle->Model();
    }

    // View reading coords (top-left origin, +y down) → desktop-local coords (centered, +y up).
    auto desktopSize = worldSize.Value();
    auto halfDesktop = desktopSize * 0.5f;
    auto halfWindow = config.worldSize * 0.5f;

    Vector3 localPosition(
        -halfDesktop.x + config.origin.x + halfWindow.x,
        halfDesktop.y - config.origin.y - halfWindow.y, 0
    );
    node->transform.SetLocalPosition(localPosition);

    Activate(*window);

    return window;
}

void Desktop::Activate(Window& window) {
    GUARD(owner)

    auto windowNode = window.Node();
    GUARD(windowNode && windowNode->Parent() == owner)

    int windowLevelZ = window.LevelZ();

    // Move the window to the end of its z-level group: it should sit just before the
    // first child whose level is strictly greater (or at the very end if none).
    auto windowNodePtr = owner->Move(*windowNode);
    GUARD(windowNodePtr)

    auto& children = owner->ChildNodes();
    size_t insertIndex = children.size();
    for (size_t i = 0; i < children.size(); i++) {
        auto otherWindow = children[i]->TypeComponent<Window>();
        if (otherWindow && otherWindow->LevelZ() > windowLevelZ) {
            insertIndex = i;
            break;
        }
    }
    owner->Insert(windowNodePtr, insertIndex);

    // Activation is exclusive within a z-level group. Other groups are untouched.
    for (auto& _window : Windows()) {
        GUARD_CONTINUE(_window->LevelZ() == windowLevelZ)
        _window->isActive = (_window.get() == &window);
    }
}

void Desktop::Close(Window& window) {
    GUARD(owner)

    int windowLevelZ = window.LevelZ();

    auto windowNode = window.Node();
    GUARD(windowNode)

    windowNode->Destroy();

    Window* newFrontWindow{};
    for (auto& _window : Windows()) {
        GUARD_CONTINUE(_window.get() != &window)
        GUARD_CONTINUE(_window->LevelZ() == windowLevelZ)
        auto node = _window->Node();

        GUARD_CONTINUE(node && !node->IsDestroyed())
        newFrontWindow = _window.get();
    }

    if (newFrontWindow) {
        Activate(*newFrontWindow);
    }
}

Window* Desktop::ActiveWindow(WindowLevel const& windowLevel) const {
    int z = windowLevel.Z();
    for (auto& window : Windows()) {
        GUARD_CONTINUE(window)
        GUARD_CONTINUE(window->LevelZ() == z)
        GUARD_CONTINUE(window->isActive.Value())
        return window.get();
    }
    return nullptr;
}
