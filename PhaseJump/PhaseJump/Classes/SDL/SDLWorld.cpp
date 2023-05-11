#include "SDLWorld.h"

using namespace std;
using namespace PJ;

void SDLWorld::MainLoop()
{
    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isDone = result.status == SomeUIEventPoller::Status::Done;
}
