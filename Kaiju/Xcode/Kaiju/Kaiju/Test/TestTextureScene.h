#pragma once

#include <PhaseJump/PhaseJump.h>
#include "TestInput.h"

using namespace PJ;

class TestStateHandler : public SomeStateHandler<ButtonControl::StateType> {
    using StateType = ButtonControlStateType;

    void OnStateChange(StateType state) override {
        GUARD(owner)
        
        switch (state) {
            case StateType::Press:
                owner->transform.SetScale(Vector2::Uniform(3.0f));
                break;
            default:
                owner->transform.SetScale(Vector2::Uniform(1.0f));
                break;
        }
    }
};

class TestTextureScene : public Scene {
public:
    SP<SomeTexture> texture;
    SP<SomeSimpleSubscription<float>> valueSubscription;

    TestTextureScene() {
    }

    void LoadInto(WorldNode& root) override {
        root.name = "TestTextureScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>("Camera");
        cameraNode->Add(camera);

        auto raycaster = MAKE<SimpleRaycaster2D>();
        cameraNode->Add(raycaster);
        root.Add(cameraNode);
        
        World& world = *root.World();

        auto uiSystem = MAKE<UISystem>();
        world.Add(uiSystem);

        auto inputMap = DCAST<InputTriggerMap>(uiSystem->inputMap);
        if (inputMap) {
            inputMap->AddTrigger("mouse", [](SomeUIEvent const& event) { return Is<PointerDownUIEvent>(event); });
            inputMap->AddTrigger("north", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
                GUARDR(concreteEvent, false)

                return concreteEvent->buttonId == ControllerButtonId::North;
            });
            inputMap->AddTrigger("south", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
                GUARDR(concreteEvent, false)

                return concreteEvent->buttonId == ControllerButtonId::South;
            });
            inputMap->AddTrigger("east", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
                GUARDR(concreteEvent, false)

                return concreteEvent->buttonId == ControllerButtonId::East;
            });
            inputMap->AddTrigger("west", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerButtonDownUIEvent>(&event);
                GUARDR(concreteEvent, false)

                return concreteEvent->buttonId == ControllerButtonId::West;
            });
            inputMap->AddTrigger("left", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerAxisUIEvent>(&event);
                GUARDR(concreteEvent && concreteEvent->value <= -0.5f, false)

                return concreteEvent->axisId == ControllerAxisId::LeftX;
            });
            inputMap->AddTrigger("right", [](SomeUIEvent const& event) {
                auto concreteEvent = As<ControllerAxisUIEvent>(&event);
                GUARDR(concreteEvent && concreteEvent->value >= 0.5f, false)

                return concreteEvent->axisId == ControllerAxisId::LeftX;
            });
        }

        {
            auto font = DCAST<Font>(world.loadedResources->map["font"]["TestFont"].resource);
            if (font) {
                auto textureAtlas = font->atlas;

                if (textureAtlas && !IsEmpty(textureAtlas->Textures())) {
//                    auto taa = DCAST<TextureAtlas>(world.loadedResources->map["texture.atlas"]["TexturePacker"].resource);
//                    auto taNode = MAKE<WorldNode>();
//                    auto sr = MAKE<SpriteRenderer>(taa->Textures()[0]);
//                    taNode->Add(sr);
//                    taNode->transform.SetScale2D(Vector2(0.5, 0.5));
//                    sr->material->EnableFeature(RenderFeature::Blend, true);
//                    world.Add(taNode);

                    auto textNode = MAKE<WorldNode>();
                    world.Add(textNode);

//                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean a luctus ipsum, vel auctor orci. Vestibulum sed convallis sem. Aenean sed turpis metus. Praesent in justo laoreet, interdum ligula et, dictum magna. Duis varius in magna vitae rhoncus. Maecenas rutrum ipsum in felis ultrices viverra. Duis gravida est nec odio fermentum aliquam.\nMaecenas vehicula enim sed tellus pulvinar, quis congue purus mollis. Pellentesque felis nulla, interdum sit amet lobortis at, pulvinar vel purus. Quisque hendrerit eget velit egestas laoreet. Mauris facilisis commodo justo, at porttitor mauris facilisis vel. Integer sollicitudin convallis lacus quis sollicitudin. Vestibulum accumsan leo eu ante lobortis.";

                    auto textRenderer = MAKE<TextRenderer>(font, "This is a test of centered text\nAnd then we have more text on the next line", Vector2(400, 400));
                    textRenderer->SetLineAlignFunc(AlignFuncs::center);
                    textRenderer->SetColor(Color::black);
                    textNode->Add(textRenderer);
                    textNode->transform.SetLocalPosition(Vector3(-100, -100, 0));

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

#ifdef SLIDER
        {
            auto sliceTexture = DCAST<GLTexture>(world.loadedResources->map["texture"]["slider-track"].resource);

            auto trackNode = MAKE<WorldNode>("Slider track");
            std::array<Vector2Int, 2> slicePoints{Vector2Int(12, 12), Vector2Int(12, 12)};
            auto trackRenderer = MAKE<SlicedTextureRenderer>(sliceTexture, Vector2(300, 33), slicePoints);
            trackNode->Add(trackRenderer);

            auto trackMaterial = trackRenderer->material;
            trackMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
            trackMaterial->EnableFeature(RenderFeature::Blend, true);

            auto animateHueEffect = MAKE<AnimateHueEffect>();
            animateHueEffect->SetIsOn(true);

            auto thumbTexture = DCAST<GLTexture>(world.loadedResources->map["texture"]["slider-thumb"].resource);
            auto thumbNode = MAKE<WorldNode>("Slider thumb");
            auto thumbRenderer = MAKE<SpriteRenderer>(thumbTexture);
            thumbNode->Add(thumbRenderer);

            auto thumbMaterial = thumbRenderer->material;
            thumbMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
            thumbMaterial->EnableFeature(RenderFeature::Blend, true);

            auto sliderControl = MAKE<SliderControl>();
            sliderControl->endCapSize = 10;
            trackNode->Add(sliderControl);
            trackNode->Add(thumbNode);

            auto worldPtr = &world;
            valueSubscription = sliderControl->Value().subject.Receive([=](float value) {
                auto camera = worldPtr->MainCamera();
                GUARD(camera)
                camera->owner->transform.SetWorldPosition(Vector3(0, value * 100, 0));
            });

            ComponentTool ct;
            auto collider = MAKE<PolygonCollider2D>();
            ct.AddComponent(*thumbNode, collider);
            ct.AddComponent(*thumbNode, animateHueEffect);

            root.Add(trackNode);
        }
#endif

        //        texture = DCAST<SomeTexture>(world.loadedResources->map["texture"]["man_run0015"].resource);
        //        GUARD(texture)

                auto textureAtlas = DCAST<TextureAtlas>(world.loadedResources->map["texture.atlas"]["man-run-atlas"].resource);
//                auto textureAtlas = DCAST<TextureAtlas>(world.loadedResources->map["texture.atlas"]["TexturePacker"].resource);
                GUARD(textureAtlas);

                auto animatedTexture = MAKE<AnimatedSpriteRenderer>(*textureAtlas);


        {
            auto meshNode = MAKE<WorldNode>("Animated runner");
//            meshNode->Add(camera);
            //        auto renderer = MAKE<MeshRenderer>();
            //        meshNode->AddComponent(renderer);

//            auto renderer = MAKE<SpriteRenderer>(texture);
//            meshNode->Add(renderer);
//            meshNode->transform.value.SetRotation(Angle::DegreesAngle(45));

            auto renderer = animatedTexture;
            meshNode->Add(renderer);
            meshNode->SetScale2D(0.5);

            auto maxTextureTrueSize = textureAtlas->MaxTextureTrueSize();
//            auto frameRenderer = MAKE<SimpleGradientRenderer>(Color::red, Color::blue, Vector2(maxTextureTrueSize.x, maxTextureTrueSize.y));

            auto frameRenderer = MAKE<ColorRenderer>(Color::red, Vector2(maxTextureTrueSize.x, maxTextureTrueSize.y));

            frameRenderer->model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
                QuadFrameMeshBuilder meshBuilder(model.WorldSize(), Vector2(3, 3));
                return meshBuilder.BuildMesh();
            });
//            meshNode->Add(frameRenderer);

            auto testKeyboard = MAKE<TestInput>();
            meshNode->Add(testKeyboard);

            //        auto material = MAKE<RenderMaterial>();
            auto material = renderer->material;

            auto program = SomeShaderProgram::registry["texture.uniform"];//texture.interp.uniform"]; //"texture.uniform"
            if (program) {
                material->SetShaderProgram(program);
                //            material->textures.Add(RenderTexture(texture->glId));

                //            material->colors.Add(Color::white);
                //            material->colors.Add(Color::red);
                //            material->colors.Add(Color::white);
                //            material->colors.Add(Color::red);

                //            material->uniformColors.Add(Color::blue);
                //            material->uniformFloats.Add(0);//0.5f);

                material->EnableFeature(RenderFeature::Blend, true);

                //            QuadMeshBuilder builder(Vector2(400, 400));
                //            auto renderMesh = builder.BuildMesh();
                //            meshRenderer->material = material;
                //            meshRenderer->mesh = renderMesh;
            }
            meshNode->transform.SetWorldPosition(Vector3(200, 0, -0.2f));
            //        meshNode->transform.scale.x = 10.0f;
            //        meshNode->transform.scale.y = 10.0f;
            //        renderer->flipX = true;
            //        renderer->flipY = true;

            //        meshNode->SetActive(false);

            //        auto uiSystem = MAKE<UISystem>();
            //        root.AddComponent(uiSystem);

            auto button = MAKE<ButtonControl>();
            auto ts = MAKE<TestStateHandler>();
            auto dragHandler = MAKE<DragHandler2D>();
            auto animateHueEffect = MAKE<AnimateHueEffect>();
//            animateHueEffect->switchHandler->SetIsOn(true);

            ComponentTool ct;
            auto collider = MAKE<PolygonCollider2D>();
            ct.AddComponent(*meshNode, collider);
//                    ct.AddComponent(*meshNode, button);
            ct.AddComponent(*meshNode, dragHandler);
            ct.AddComponent(*meshNode, ts);
//            ct.AddComponent(*meshNode, animateHueEffect);

            root.Add(meshNode);
        }
    }
};
