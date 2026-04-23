#include "SDLWorld.h"
#include "Funcs.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_internal.h"
#include "SomeUIEventPoller.h"

using namespace std;
using namespace PJ;

// FUTURE: evaluate putting this on a separate thread
void SDLWorld::MainLoop() {
    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isFinished = result.state == SomeUIEventPoller::StateType::Finished;
}

void SDLWorld::SetCursor(String cursorId) {
    auto iter = sdlCursors.find(cursorId);
    GUARD(iter != sdlCursors.end() && iter->second)
    SDL_SetCursor(iter->second);
    currentCursorId = cursorId;
}

void SDLWorld::OnGo() {
    Base::OnGo();

    GUARD_LOG(window, "ERROR: SDLWorld is missing window")
    GUARD_LOG(renderContext, "ERROR: SDLWorld is missing render context")

    sdlCursors[CursorId::Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    sdlCursors[CursorId::TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    sdlCursors[CursorId::Wait] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
    sdlCursors[CursorId::Crosshair] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    sdlCursors[CursorId::Progress] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_PROGRESS);
    sdlCursors[CursorId::ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE);
    sdlCursors[CursorId::ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NESW_RESIZE);
    sdlCursors[CursorId::ResizeEastWest] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_EW_RESIZE);
    sdlCursors[CursorId::ResizeNorthSouth] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NS_RESIZE);
    sdlCursors[CursorId::Move] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_MOVE);
    sdlCursors[CursorId::NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NOT_ALLOWED);
    sdlCursors[CursorId::Pointer] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    sdlCursors[CursorId::ResizeNorthwest] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NW_RESIZE);
    sdlCursors[CursorId::ResizeNorth] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_N_RESIZE);
    sdlCursors[CursorId::ResizeNortheast] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NE_RESIZE);
    sdlCursors[CursorId::ResizeEast] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_E_RESIZE);
    sdlCursors[CursorId::ResizeSoutheast] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SE_RESIZE);
    sdlCursors[CursorId::ResizeSouth] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_S_RESIZE);
    sdlCursors[CursorId::ResizeSouthwest] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SW_RESIZE);
    sdlCursors[CursorId::ResizeWest] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_W_RESIZE);

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
