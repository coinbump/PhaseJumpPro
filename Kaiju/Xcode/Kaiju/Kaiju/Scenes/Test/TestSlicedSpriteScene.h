//
//  TestColorVaryScene.h
//  Kaiju
//
//  Created by Jeremy Vineyard on 4/18/23.
//

#ifndef TestSlicedSpriteScene_h
#define TestSlicedSpriteScene_h

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

class TestSlicedSpriteScene : public Scene {
public:
    SP<GLTexture> texture;

    TestSlicedSpriteScene(SP<GLTexture> texture) : texture(texture) {
    }

    void LoadInto(World& world) {
        if (nullptr == texture) { return; }

        auto meshNode = MAKE<WorldNode>();

        std::array<Vector2Int, 2> slicePoints = {Vector2Int(25, 25), Vector2Int(25, 25)};
        auto renderer = MAKE<SlicedTextureRenderer>(texture, Vector2(300, 300), slicePoints);
        meshNode->AddComponent(renderer);

        auto material = renderer->material;
        
        auto program = GLShaderProgram::registry["texture.uniform"];
        if (program) {
            material->shaderProgram = program;
//            material->features[RenderFeatures::Blend] = RenderFeatureStatus::Enable;
        }
        meshNode->transform->position.z = -0.1f;
//        meshNode->transform->rotation.z = -45.0f;

        world.Add(meshNode);
    }
};

#endif
