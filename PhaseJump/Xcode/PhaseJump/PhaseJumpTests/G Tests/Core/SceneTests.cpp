#include "gtest/gtest.h"
#include "Scene.h"

using namespace PJ;
using namespace std;

namespace SceneTests {
}

using namespace SceneTests;

TEST(Scene, IsLoaded)
{
    SP<Scene> scene = MAKE<Scene>();

    EXPECT_FALSE(scene->IsLoaded());
    scene->Start();
    EXPECT_TRUE(scene->IsLoaded());
}
