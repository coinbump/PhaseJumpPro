#include "SDLWorld.h"
#include "Funcs.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_internal.h"

using namespace std;
using namespace PJ;

// FUTURE: evaluate putting this on a separate thread
void SDLWorld::MainLoop() {
    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isFinished = result.state == SomeUIEventPoller::StateType::Finished;
}

void SDLWorld::OnGo() {
    Base::OnGo();

    GUARD_LOG(window, "ERROR: SDLWorld is missing window")
    GUARD_LOG(renderContext, "ERROR: SDLWorld is missing render context")

    /*
     Disable Ctrl-Tab shortcuts in ImGui so we can use them

     Reference:
     https://github.com/ocornut/imgui/issues/3255
     */
    if (GImGui) {
        ImGuiContext& imContext = *GImGui;
        imContext.ConfigNavWindowingKeyNext = 0;
        imContext.ConfigNavWindowingKeyPrev = 0;
    }

#ifdef LAUNCH_PROFILE
    DevLaunchProfile();
#endif

    bool didRender = false;

    startTime = SDL_GetTicks();

    while (!isFinished) {
        uint64_t currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - startTime) / 1000.0; // Convert to seconds.

        // Avoid flash before first frame
        didRender = true;

        startTime = currentTime;

        {
#ifdef PROFILE
            DevProfiler devProfiler("SDL-OnUpdate", [](String value) { std::cout << value; });
#endif

            // Break down large time deltas due to hitching/debugger pause into multiple time steps
            while (deltaTime > maxTimeDelta) {
                OnUpdate(TimeSlice(maxTimeDelta));
                deltaTime -= maxTimeDelta;
            }
            OnUpdate(TimeSlice(deltaTime));
        }

        {
#ifdef PROFILE
            DevProfiler devProfiler("SDL-Render", [](String value) { std::cout << value; });
#endif
            Render();
        }

        {
#ifdef PROFILE
            DevProfiler devProfiler("SDL-MainLoop", [](String value) { std::cout << value; });
#endif
            MainLoop();
        }
    }
    SDL_Quit();
}
