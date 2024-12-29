#include "SomePlatformWindow.h"
#include "World.h"

using namespace std;
using namespace PJ;

void SomePlatformWindow::SetWorld(SP<World> value) {
    GUARD(world != value)
    if (world) {
        world->window = {};
    }

    world = value;

    GUARD(world)
    world->window = this;
}
