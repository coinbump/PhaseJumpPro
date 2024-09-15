#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif
#include "TestInput.h"

using namespace std;
using namespace PJ;

//class TestStateHandler : public SomeStateHandler<ButtonControl::StateType> {
//    using StateType = ButtonControlStateType;
//
//    void OnStateChange(StateType state) override {
//        GUARD(owner)
//        
//        switch (state) {
//            case StateType::Press:
//                owner->transform.SetScale(Vector2::Uniform(3.0f));
//                break;
//            default:
//                owner->transform.SetScale(Vector2::Uniform(1.0f));
//                break;
//        }
//    }
//};

namespace PJ {
}

#if FALSE
// quickBuilder <- allows you to switch which UI you use

// PROBLEM: this is still too complex
AddSlider(Axis2D::X, "slider-track", {{12, 12}, {12, 12}}, "slider-thumb", 10)
    .SetLocalPosition(Vector3(0, -200, 0);
#endif

class TestTextureScene : public Scene {
public:
    SP<SomeTexture> texture;
    SP<SomeSimpleSubscription<float>> valueSubscription;

    TestTextureScene() {
    }

    void LoadInto(WorldNode& root) override {
        root.name = "TestTextureScene";

//        WorldNode& cameraNode = root.And("Camera");
//        cameraNode.With<OrthoCamera>();//.SetHalfHeight(owner->World()->renderContext->PixelSize().y);
//        cameraNode.With<SimpleRaycaster2D>();

        World& world = *root.World();

#define BUFFER
#ifdef BUFFER
        auto& bufferCamera = root.With<OrthoCamera>();
        auto textureBuffer = world.renderContext->renderEngine->MakeTextureBuffer();
        textureBuffer->Build(Vector2Int(2000, 1000));
//        textureBuffer->clearColor = Color::blue;
        bufferCamera.renderContext = textureBuffer;

        auto& bufferRenderNode = root.And("Buffer render");
        auto& spriteRenderer = bufferRenderNode.With<SpriteRenderer>(textureBuffer->Texture());

        auto program = SomeShaderProgram::registry.find("chromaticAberrationSimple");
//        auto program = SomeShaderProgram::registry.find("texture.uniform");
        if (program != SomeShaderProgram::registry.end()) {
            spriteRenderer.model.material->SetShaderProgram(program->second);
        }

        // TODO: this is cheating the UV coordinates for now, don't forget
        bufferRenderNode.transform.SetRotation(Vector3(0, 0, -180));
#endif
        
//        auto inputMap = DCAST<InputTriggerMap>(uiSystem->inputMap);
//        if (inputMap) {
//            inputMap->AddTrigger("mouse", [](SomeUIEvent const& event) { return Is<PointerDownUIEvent>(event); });
//            inputMap->AddTrigger("north", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
//                GUARDR(concreteEvent, false)
//
//                return concreteEvent->buttonId == ControllerButtonId::North;
//            });
//            inputMap->AddTrigger("south", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
//                GUARDR(concreteEvent, false)
//
//                return concreteEvent->buttonId == ControllerButtonId::South;
//            });
//            inputMap->AddTrigger("east", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
//                GUARDR(concreteEvent, false)
//
//                return concreteEvent->buttonId == ControllerButtonId::East;
//            });
//            inputMap->AddTrigger("west", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
//                GUARDR(concreteEvent, false)
//
//                return concreteEvent->buttonId == ControllerButtonId::West;
//            });
//            inputMap->AddTrigger("left", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerAxisUIEvent>(&event);
//                GUARDR(concreteEvent && concreteEvent->value <= -0.5f, false)
//
//                return concreteEvent->axisId == ControllerAxisId::LeftX;
//            });
//            inputMap->AddTrigger("right", [](SomeUIEvent const& event) {
//                auto concreteEvent = As<ControllerAxisUIEvent>(&event);
//                GUARDR(concreteEvent && concreteEvent->value >= 0.5f, false)
//
//                return concreteEvent->axisId == ControllerAxisId::LeftX;
//            });
//        }

        for (int i = 0; i < 1; i++) {
//            auto font = DCAST<Font>(world.loadedResources->map["font"]["TestFont"].resource);
            auto font = DCAST<Font>(world.FindFontWithSize(32));

            if (font) {
                auto textureAtlas = font->atlas;

                if (textureAtlas && !IsEmpty(textureAtlas->Textures())) {
                    WorldNode& textNode = world.And();
                    textNode.With<TextRenderer>(font, "This is a test of centered text\nAnd then we have more text on the next line", Vector2(400, 400))
                        .SetLineAlignFunc(AlignFuncs::center)
                        .SetTextColor(Color::blue)
                        .SizeToFit();
                    textNode.transform.SetLocalPosition(Vector3(-100 + 20*i, -100 + 20*i, 0));
                    textNode.With<AnimateHueEffect>().SetIsOn(true);

//                    textRenderer->SetModifyColorsFunc([](TextRenderer& textRenderer, VectorList<RenderColor>& colors) {
//                        auto const& renderChars = textRenderer.RenderChars();
//                        auto const& text = textRenderer.Text();
//
//                        // TODO: this doesn't work because \n isn't rendered in the mesh
//                        for (size_t i = 0; i < renderChars.size(); i++) {
//                            auto renderChar = renderChars[i];
//                            auto _char = text[renderChar.sourceIndex];
//
//                            if (_char == 'e') {
//                                size_t vertexIndex = renderChar.vertexIndex;
//                                colors[vertexIndex + 0] = Color::green;
//                                colors[vertexIndex + 1] = Color::red;
//                                colors[vertexIndex + 2] = Color::green;
//                                colors[vertexIndex + 3] = Color::red;
//
////                                std::cout << "Vertex: " << vertexIndex << " Total: " << colors.size() << std::endl;
//                            }
//                        }
//                    });
//
//                    textRenderer->SetModifyVerticesFunc([](TextRenderer& textRenderer, VectorList<Vector3>& vertices) {
//                        auto const& renderChars = textRenderer.RenderChars();
//                        auto const& text = textRenderer.Text();
//
//                        // TODO: this doesn't work because \n isn't rendered in the mesh
//                        for (size_t i = 0; i < renderChars.size(); i++) {
//                            auto renderChar = renderChars[i];
//                            auto _char = text[renderChar.sourceIndex];
//
//                            if (_char == 'e') {
////                                Matrix4x4 matrix;
////                                matrix.LoadXRadRotation(Angle::DegreesAngle(45).Radians());
//
//                                size_t vertexIndex = renderChar.vertexIndex;
//
//                                vertices[vertexIndex + 0] += Vector3(-3, 3, 0);
//                                vertices[vertexIndex + 1] += Vector3(3, 3, 0);
//                                vertices[vertexIndex + 2] += Vector3(-3, -3, 0);
//                                vertices[vertexIndex + 3] += Vector3(3, -3, 0);
//
////                                vertices[vertexIndex + 0] = matrix.MultiplyPoint(vertices[vertexIndex + 0]);
////                                vertices[vertexIndex + 1] = matrix.MultiplyPoint(vertices[vertexIndex + 1]);
////                                vertices[vertexIndex + 2] = matrix.MultiplyPoint(vertices[vertexIndex + 2]);
////                                vertices[vertexIndex + 3] = matrix.MultiplyPoint(vertices[vertexIndex + 3]);
//                            }
//                        }
//                    });

                }
            }
        }

        // Test multi camera render
//        auto camera2 = SCAST<SomeCamera>(MAKE<OrthoCamera>());
//        auto cameraNode2 = MAKE<WorldNode>();
//        cameraNode2->Add(camera2);
//        cameraNode2->transform.SetWorldPosition(Vector3(10, 10, 0));
//        root.Add(cameraNode2);

#define SLIDER

#ifdef SLIDER
        {
#if FALSE
            auto slider = DuckUI::AddSlider(world, "My slider", 0, 10, [](float value) {
                // Receive slider value here
            });

#endif
            QB(root)
                .Slider("My slider", {500, 33}, [](float value) {
                // Receive slider value here
                std::cout << "Slider: " << value << std::endl;
            })
            .SetLocalPosition({0, -200, 0});

            QB(root)
                .Slider("Vertical slider", {33, 500}, [](float value) {
                // Receive slider value here
                std::cout << "VerticalSlider: " << value << std::endl;
            })
            .SetLocalPosition({-300, 000, 0});
        }
#endif

        //        texture = DCAST<SomeTexture>(world.loadedResources->map["texture"]["man_run0015"].resource);
        //        GUARD(texture)

                auto textureAtlas = DCAST<TextureAtlas>(world.loadedResources->map["texture.atlas"]["man-run-atlas"].resource);
//                auto textureAtlas = DCAST<TextureAtlas>(world.loadedResources->map["texture.atlas"]["TexturePacker"].resource);
                GUARD(textureAtlas);

                auto animatedTexture = MAKE<AnimatedSpriteRenderer>(*textureAtlas);


        {
            WorldNode& meshNode = root.And("Animated runner");
//            meshNode->Add(camera);
            //        auto renderer = MAKE<MeshRenderer>();
            //        meshNode->With(renderer);

//            auto renderer = MAKE<SpriteRenderer>(texture);
//            meshNode->Add(renderer);
//            meshNode->transform.value.SetRotation(Angle::DegreesAngle(45));

            auto renderer = animatedTexture;
            animatedTexture->SetColor(Color::blue);
            meshNode.Add(renderer);
            meshNode.SetScale2D(0.5);

            WorldNode& cameraNode = meshNode.And("Camera");
            auto camera = cameraNode.WithPtr<OrthoCamera>();
            camera->SetHalfHeight(owner->World()->renderContext->PixelSize().y / 2.0f);
            cameraNode.With<SimpleRaycaster2D>();

//            updatables.Add([camera](TimeSlice time) {
//                GUARDR(camera->halfHeight, FinishType::Continue)
//                camera->SetHalfHeight(*camera->halfHeight + 10.0f * time.delta);
//
//                return FinishType::Continue;
//            });

#if FALSE
            updatables.Add(MAKE<PeriodicRepeater>(3, 2, []() {... })
            updatables.Add(
                           RunNTimes(3, DelayDriver(10, []() {
                               // Do something
                           })
                                       )
#endif

//            updatables.AddContinue([camera](TimeSlice time) {
//                GUARD(camera->halfHeight)
//                camera->SetHalfHeight(*camera->halfHeight + 10.0f * time.delta);
//            });
           updatables.AddTimed(3, [camera](TimeSlice time) {
               GUARDR(camera->halfHeight, FinishType::Continue)
               camera->SetHalfHeight(*camera->halfHeight + 50.0f * time.delta);

               return FinishType::Continue;
           });

            auto maxTextureTrueSize = textureAtlas->MaxTextureTrueSize();
//            auto frameRenderer = MAKE<SimpleGradientRenderer>(Color::red, Color::blue, Vector2(maxTextureTrueSize.x, maxTextureTrueSize.y));

            auto frameRenderer = MAKE<ColorRenderer>(Color::red, Vector2(maxTextureTrueSize.x, maxTextureTrueSize.y));

            frameRenderer->model.SetBuildMeshFunc([](RendererModel const& model) {
                QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(3, 3));
                return meshBuilder.BuildMesh();
            });
//            meshNode->Add(frameRenderer);

            auto testKeyboard = MAKE<TestInput>();
            meshNode.Add(testKeyboard);

            //        auto material = MAKE<RenderMaterial>();
            auto material = renderer->model.material;

//            auto program = SomeShaderProgram::registry["texture.uniform"];//texture.interp.uniform"]; //"texture.uniform"
//            if (program) {
//                material->SetShaderProgram(program);
//                //            material->textures.Add(RenderTexture(texture->glId));
//
//                //            material->colors.Add(Color::white);
//                //            material->colors.Add(Color::red);
//                //            material->colors.Add(Color::white);
//                //            material->colors.Add(Color::red);
//
//                //            material->uniformColors.Add(Color::blue);
//                //            material->uniformFloats.Add(0);//0.5f);
//
//                material->EnableFeature(RenderFeature::Blend, true);
//
//                //            QuadMeshBuilder builder(Vector2(400, 400));
//                //            auto renderMesh = builder.BuildMesh();
//                //            meshRenderer->material = material;
//                //            meshRenderer->mesh = renderMesh;
//            }
            meshNode.transform.SetWorldPosition(Vector3(200, 0, -0.2f));
            //        meshNode->transform.scale.x = 10.0f;
            //        meshNode->transform.scale.y = 10.0f;
            //        renderer->flipX = true;
            //        renderer->flipY = true;

            //        meshNode->SetActive(false);


//            auto button = MAKE<ButtonControl>();
//            auto ts = MAKE<TestStateHandler>();
            auto dragHandler = MAKE<DragHandler2D>();
//            auto animateHueEffect = MAKE<AnimateHueEffect>();
//            animateHueEffect->switchHandler->SetIsOn(true);

            ComponentTool ct;
            auto collider = MAKE<PolygonCollider2D>();
            ct.AddComponent(meshNode, collider);
//                    ct.AddComponent(*meshNode, button);
            ct.AddComponent(meshNode, dragHandler);
//            ct.AddComponent(meshNode, ts);
//            ct.AddComponent(*meshNode, animateHueEffect);
        }
    }
};
