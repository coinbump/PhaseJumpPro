#include "SDLWorld.h"
#include "Funcs.h"

using namespace std;
using namespace PJ;

// FUTURE: evaluate putting this on a separate thread
void SDLWorld::MainLoop() {
    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isFinished = result.state == SomeUIEventPoller::StateType::Finished;
}

VectorList<uint32_t> const triangles{ 0, 1, 2, 1, 3, 2 };
VectorList<Vector2> const uvs{ Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), Vector2(1, 1) };

class QuadMeshBuilderA : public SomeMeshBuilder {
public:
    Vector2 worldSize{ 10.0f, 10.0f };

    QuadMeshBuilderA(Vector2 worldSize = vec2One) :
        worldSize(worldSize) {}

    Mesh BuildMesh() override {
        float halfX = worldSize.x / 2.0f;
        float halfY = worldSize.y / 2.0f;
        VectorList<Vector3> vertices{ Vector3{ halfX * vecLeft, halfY * vecUp, 0 },
                                      Vector3{ halfX * vecRight, halfY * vecUp, 0 },
                                      Vector3{ halfX * vecLeft, halfY * vecDown, 0 },
                                      Vector3{ halfX * vecRight, halfY * vecDown, 0 } };

        Mesh result;

        return result;
    }
};

class QuadMeshBuilderB : public SomeMeshBuilder {
public:
    Vector2 worldSize{ 10.0f, 10.0f };

    QuadMeshBuilderB(Vector2 worldSize = vec2One) :
        worldSize(worldSize) {}

    Mesh BuildMesh() override {
        float halfX = worldSize.x / 2.0f;
        float halfY = worldSize.y / 2.0f;
        //        VectorList<Vector3> vertices{ Vector3{ halfX * vecLeft, halfY * vecUp, 0 },
        //                                      Vector3{ halfX * vecRight, halfY * vecUp, 0 },
        //                                      Vector3{ halfX * vecLeft, halfY * vecDown, 0 },
        //                                      Vector3{ halfX * vecRight, halfY * vecDown, 0 } };

        // TODO: the problem with this is that it isn't generally usable because the UVs aren't
        // going to be the same for all the quad mesh textures, so....
        Mesh result({ Vector3{ halfX * vecLeft, halfY * vecUp, 0 },
                      Vector3{ halfX * vecRight, halfY * vecUp, 0 },
                      Vector3{ halfX * vecLeft, halfY * vecDown, 0 },
                      Vector3{ halfX * vecRight, halfY * vecDown, 0 } });
        //        result.SetSharedTriangles(&triangles);
        //        result.SetSharedUVs(&uvs);

        return result;
    }
};

struct Vector3TempArray {
public:
    using This = Vector3TempArray;

    float x = 0;
    float y = 0;
    float z = 0;

    constexpr float& operator[](size_t index) {
        float* indices[3] = { &x, &y, &z };
        return *indices[index];
    }
};

struct Vector3Switch {
public:
    using This = Vector3Switch;

    float x = 0;
    float y = 0;
    float z = 0;

    constexpr float& operator[](size_t index) {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return z;
        }
    }
};

struct Vector3Ternary {
public:
    using This = Vector3Ternary;

    float x = 0;
    float y = 0;
    float z = 0;

    constexpr float& operator[](size_t index) {
        return index == 0 ? x : index == 1 ? y : z;
    }
};

void SDLWorld::GoInternal() {
    Base::GoInternal();

    DevABProfiler profile(
        "Vector[]- switch",
        []() {
            Vector3Switch value;
            static bool flag = false;
            auto foo = [](int a, int b) {
                flag = !flag;
                return flag ? a * b : a + b;
            };
            For(0, 1000, [&]() {
                value[1] = foo(value[0], value[1]);
                value[0] = foo(value[1], value[0]);
            });
        },
        "Vector[]- ternary",
        []() {
            Vector3Ternary value;
            static bool flag = false;
            auto foo = [](int a, int b) {
                flag = !flag;
                return flag ? a * b : a + b;
            };
            For(0, 1000, [&]() {
                value[1] = foo(value[0], value[1]);
                value[0] = foo(value[1], value[0]);
            });
        },
        100'000, [](DevABProfiler::Result result) { std::cout << result.formattedString; }
    );

    bool didRender = false;

    startTime = SDL_GetTicks();

    while (!isFinished) {
        // This doesn't work
        //        auto displayMode = SDL_GetWindowFullscreenMode(window);
        //        if (displayMode) {
        //            SetRenderRate(displayMode->refresh_rate);
        //            cout << "Render rate: " << displayMode->refresh_rate << std::endl;
        //        }

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
