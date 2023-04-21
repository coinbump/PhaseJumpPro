#include "SDLWorld.h"

using namespace std;
using namespace PJ;

void SDLWorld::mainLoop()
{
    if (!renderContext) { return; }

    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isDone = result.status == SomeUIEventPoller::Status::Done;

    Render();
}

void SDLWorld::ProcessUIEvents(VectorList<std::shared_ptr<SomeUIEvent>> const& uiEvents) {
}
