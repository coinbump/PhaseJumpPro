#include "SDLWorld.h"
#include "Funcs.h"

using namespace std;
using namespace PJ;

void SDLWorld::MainLoop() {
    auto result = uiEventPoller->PollUIEvents();

    ProcessUIEvents(result.uiEvents);

    isDone = result.status == SomeUIEventPoller::Status::Done;
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

void SDLWorld::Run() {
    startTime = SDL_GetTicks();

    DevABProfiler profile(
        "Func for- non constexpr",
        []() {
            String value;
            // ForNoConstexpr(0, 1000, [&]() { value += "a"; });
        },
        "Func for",
        []() {
            String value;
            For(0, 1000, [&]() { value += "a"; });
        },
        100'000, [](DevABProfiler::Result result) { std::cout << result.formattedString; }
    );

    bool didRender = false;

    while (!isDone) {
        auto currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - startTime) / 1000.0; // Convert to seconds.

        // TODO: revisit the idea of frame caps
        if (!didRender || deltaTime >= 1.0f / 240.0f) {
            // Avoid flash before first frame
            didRender = true;

            startTime = currentTime;

            {
#ifdef PROFILE
                DevProfiler devProfiler("SDL-OnUpdate", [](String value) { std::cout << value; });
#endif

                OnUpdate(TimeSlice(deltaTime));
            }

            {
#ifdef PROFILE
                DevProfiler devProfiler("SDL-Render", [](String value) { std::cout << value; });
#endif
                Render();
            }
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
