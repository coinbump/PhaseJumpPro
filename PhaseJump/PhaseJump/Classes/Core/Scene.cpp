#include "Scene.h"
#include "World.h"

using namespace std;
using namespace PJ;

void Scene::Start() {
    Base::Start();

    GUARD(!isLoaded)

    // Set flag early for unit tests
    isLoaded = true;

    GUARD(owner && owner->world);
    LoadInto(*owner);
}
