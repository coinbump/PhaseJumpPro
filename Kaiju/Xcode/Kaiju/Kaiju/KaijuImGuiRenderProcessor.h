#pragma once

#include <PhaseJump/PhaseJump.h>
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestTextureScene.h"
#include "TestSlicedTextureScene.h"
#include "TestAnimatedTexturesScene.h"
#include "TestAudioScene.h"
#include "TestZOrderScene.h"
#include "TestEmittersScene.h"
#include "TestEditBezierScene.h"
#include "TestUIScene.h"
#include "TestThemeScene.h"
#include "ExampleLifeScene.h"

using namespace PJ;

class KaijuSceneClass : public TypeClass<Scene> {
public:
    KaijuSceneClass(String id, String name, FactoryFunc allocator) : TypeClass<Scene>(id, allocator) {
        core.name = name;
    }
};

void DrawSelectionWindow(WorldNode& node) {
    auto& components = node.Components();

    static char text[1000];

    std::ostringstream ss;
    ss << node.transform.LocalPosition();

    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Position: %s", ss.str().c_str());

    size_t i = 0;
    std::for_each(components.begin(), components.end(), [&](SP<SomeWorldComponent> component) {
        auto typeName = typeid(component).name();

        auto textRenderer = As<TextRenderer>(component.get());
        if (textRenderer) {
            std::strcpy(text, textRenderer->Text().c_str());

            ImGui::InputText("Text", text, 1000);

            textRenderer->SetText(text);
        }

//        GUARD(typeName.size() > 0)

        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", typeName);

        i++;
    });
}

void DrawTree(WorldNode* node, String name) {
    GUARD(node);

    // TODO: move this to a system?
    if (ImGui::TreeNodeEx(name.c_str())) {
        bool isScene = !IsEmpty(node->GetComponents<Scene>());
        bool isCamera = !IsEmpty(node->GetComponents<SomeCamera>());
        bool isVIP = node->typeTags.contains("editor.vip");

        if (!(isScene || isCamera || isVIP)) {
            ImGui::SameLine();
            //        if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
            //            pressed_count += 1;

            // FUTURE: use imageButton
            if (ImGui::SmallButton(node->IsEnabled() ? "on" : "off")) {
                node->ToggleEnable();
            }
        }

        Tags nameCounts;

        for (auto & childNode : node->ChildNodes()) {
            String name = childNode->name.size() > 0 ? childNode->name : "???";
            
            auto nameCount = nameCounts.SafeValue<int>(name);
            nameCounts.Insert(name, nameCount + 1);

            if (nameCount > 0) {
                std::ostringstream ss;
                ss << name << " (" << MakeString(nameCount + 1) << ")";
                name = ss.str();
            }

            DrawTree(childNode.get(), name);
        }
        ImGui::TreePop();
    }
}

class KaijuImGuiRenderProcessor : public RenderProcessor, public SomeKeyUIEventsResponder {
public:
    using Base = RenderProcessor;

    List<SP<KaijuSceneClass>> sceneClasses;
    bool isWindowConfigured = false;
    bool isToolActive = false;
    bool isUIVisible = true;

    World& world;

    KaijuImGuiRenderProcessor(World& world) : world(world) {
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.texture", "Test Texture", []() { return MAKE<TestTextureScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.slicedTexture", "Test Sliced Texture", []() { return MAKE<TestSlicedTextureScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.gradients", "Test Gradients", []() { return MAKE<TestGradientsScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.meshPath", "Test Mesh Path", []() { return MAKE<TestMeshPathScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.audio", "Test Audio", []() { return MAKE<TestAudioScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.emitters", "Test Emitters", []() { return MAKE<TestEmittersScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.zOrder", "Test ZOrder", []() { return MAKE<TestZOrderScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.editBezier", "Test Edit Bezier", []() { return MAKE<TestEditBezierScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.theme", "Test Theme", []() { return MAKE<TestThemeScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("test.ui", "Test UI", []() { return MAKE<TestUIScene>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("example.life", "Life Example", []() { return MAKE<ExampleLifeScene>(); }));
    }

    // MARK: RenderProcessor

    // TODO: rethink where RenderInto is getting called
    void Process(RenderSystemModel& systemModel) override {
        auto world = &this->world;
        GUARD(world)

        GUARD(isUIVisible)

        if (!isWindowConfigured) {
            isWindowConfigured = true;

            ImGui::SetNextWindowSize(ImVec2(300, 500));
            ImGui::SetNextWindowPos(ImVec2(20, 20));
        }

        ImGui::Begin("Kaiju", &isToolActive, ImGuiWindowFlags_MenuBar);

//        static bool menu = false;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Examples"))
            {
                if (ImGui::MenuItem("Do something", "C", false)) {
                    std::cout << "DO SOMETHING" << std::endl;
                }

                ImGui::SeparatorText("Mini apps");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Scenes");

        int fps = world->renderStats.SafeValue<int>("fps");
        VectorList<SP<WorldNode>> nodes;
        CollectBreadthFirstTree(world->root, nodes);

        String fpsString = MakeString(fps);
        String fpsTitle = String("FPS: ") + fpsString;
        ImGui::TextColored(fps < 30 ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), "%s", fpsTitle.c_str());

        if (world->IsPaused()) {
            if (ImGui::Button("Play")) {
                world->Play();
            }
        } else {
            if (ImGui::Button("Pause")) {
                world->Pause();
            }
        }

        // FUTURE: support RayLib style UI building of Label, etc.

        // TODO: can we make it buildable with modifiers, like SwiftUI?
//        uiPainter.DrawValueCount("Draw count", world->renderStats.SafeValue<int>("draw.count"));

        {
            int drawCount = world->renderStats.SafeValue<int>("draw.count");
            String drawCountString = MakeString(drawCount);
            String drawCountTitle = String("Draw Count: ") + drawCountString;
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", drawCountTitle.c_str());
        }
        
        {
            int nodeCount = world->renderStats.SafeValue<int>("node.count");
            String drawCountString = MakeString(nodeCount);
            String drawCountTitle = String("Node Count: ") + drawCountString;
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", drawCountTitle.c_str());
        }

        ImGui::BeginChild("Scrolling");
        if (ImGui::CollapsingHeader("Scenes")) {
            for (auto& sceneClass : sceneClasses) {
                auto name = sceneClass->core.name;

                // ImGui requires that UI elements have names (unless you specify ##)
                GUARD_CONTINUE(name.size() > 0)

                if (ImGui::Button(name.c_str())) {
                    auto scene = sceneClass->Make();
                    if (scene) {
                        auto removingSystems = Filter(world->Systems(), [](SP<SomeWorldSystem> system) {
                            return !system->typeTags.contains("editor.vip");
                        });

                        world->Remove(removingSystems);
                        world->RemoveAllNodes();

                        auto sceneNode = MAKE<WorldNode>();
                        sceneNode->Add(scene);
                        world->Add(sceneNode);
                    }
                }
            }
        }

        if (ImGui::CollapsingHeader("Resources")) {
            for (auto& type : world->loadedResources->map) {
                auto name = type.first.c_str();
                GUARD_CONTINUE(name)

                if (ImGui::CollapsingHeader(name)) {
                    for (auto& res : type.second) {
                        auto resourceId = res.second.id;
                        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s, Used: %ld", resourceId.c_str(), res.second.resource.use_count());

                        if (String(name) == String("texture")) {
                            ImGui::SameLine();
                            
                            String addButtonName("+");
                            addButtonName += "##texture-";
                            addButtonName += resourceId;

                            if (ImGui::SmallButton(addButtonName.c_str())) {
                                world->AddNode()
                                    .AddComponent<SpriteRenderer>(SCAST<SomeTexture>(res.second.resource));
                            }
                        }
                    }
                }
            }
        }

        DrawTree(world->root.get(), "Root");

        if (ImGui::CollapsingHeader("Render Pipeline")) {
            auto renderSystem = world->GetSystem<RenderWorldSystem>();

            auto& systems = world->Systems();

            static std::array<bool, 100> systemBools;
            static std::array<bool, 100> bools;

            {
                static bool didBoolsGo = false;
                if (!didBoolsGo) {
                    didBoolsGo = true;

                    bools.fill(true);
                }
            }

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Systems");

            for (size_t i = 0; i < systems.size(); i++) {
                auto& system = systems[i];
                GUARD_CONTINUE(system->name.size() > 0)

                ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", system->name.c_str());

                //            ImGui::Checkbox(system->name.c_str(), &systemBools[i]);
                //            processor->SetActive(bools[i]);
            }

            if (renderSystem) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Render Pipeline");

                auto const& processors = renderSystem->Processors();

                static bool didBoolsGo = false;
                if (!didBoolsGo) {
                    didBoolsGo = true;

                    bools.fill(true);

                    for (size_t i = 0; i < processors.size(); i++) {
                        bools[i] = processors[i]->IsEnabled();
                    }
                }

                for (size_t i = 0; i < processors.size(); i++) {
                    auto& processor = processors[i];
                    GUARD_CONTINUE(processor->name.size() > 0)

                    ImGui::Checkbox(processor->name.c_str(), &bools[i]);

                    processor->Enable(bools[i]);
                }
            } else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "ERROR: Missing render system");
            }
        }

        if (!IsEmpty(nodes)) {
            auto found = std::find_if(nodes.begin(), nodes.end(), [](SP<WorldNode> node) {
                return nullptr != node->GetComponent<TextRenderer>();
            });
            if (found != nodes.end()) {
                DrawSelectionWindow(**found);
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }

    // MARK: SomeKeyUIEventsResponder

    void OnKeyDown(KeyDownUIEvent event) override {
        if (event.keyCode.value == '\t') {
            isUIVisible = !isUIVisible;
        }
    }
};

