#include "SimpleAnimationController.h"
#include "Renderer.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

SimpleAnimationController::SimpleAnimationController() {
    setRendererFunc = [](This& controller, auto renderer) {
        auto target = controller.Target();
        GUARD(target)

        target->RemoveType<Renderer>();
        target->Add(renderer);
    };

    StateMachine<String>::OnStateChangeFunc onStateChangeFunc = [this](auto& states) {
        try {
            // Don't build the renderer until we are in the world
            GUARD(owner && owner->World())

            auto state = states.State();
            auto& buildRendererFunc = buildRendererFuncs.at(state);

            auto renderer = buildRendererFunc(*this);

            GUARD(renderer && setRendererFunc)
            setRendererFunc(*this, renderer);
        } catch (...) {}
    };
    states.OverrideOnStateChangeFunc(onStateChangeFunc);

    // Build the renderer once we are in the world
    WorldComponentFunc awakeFunc = [this](auto& component) { states.ForceOnStateChange(); };
    Override(this->awakeFunc, awakeFunc);
}

void SimpleAnimationController::Add(String state, BuildRendererFunc buildRendererFunc) {
    states.Add(state);
    buildRendererFuncs.insert_or_assign(state, buildRendererFunc);
}
