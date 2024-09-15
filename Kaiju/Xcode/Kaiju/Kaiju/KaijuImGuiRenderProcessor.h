#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestTextureScene.h"
#include "TestSlicedTextureScene.h"
#include "TestAnimateApp.h"
#include "TestAudioScene.h"
#include "TestZOrderScene.h"
#include "TestEmittersScene.h"
#include "TestEditBezierScene.h"
#include "TestUIScene.h"
#include "TestThemeScene.h"
#include "ExampleLifeScene.h"
#include "KaijuWorldSystem.h"

using namespace PJ;

class KaijuSceneClass : public TypeClass<Scene> {
public:
    KaijuSceneClass(String id, String name, FactoryFunc allocator) : TypeClass<Scene>(id, allocator) {
        core.name = name;
    }
};

void DrawSelectionWindow(WorldNode& node) {
    // TODO: which is selected?
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

struct DeleteNodeCommandCore {
    SP<WorldNode> node;
    WorldNode* parent{};
};

void BuildInspectedNodeWindow(KaijuWorldSystem& system) {
    GUARD(system.inspectedNode)

    if (!system.isInspectWindowConfigured) {
        system.isInspectWindowConfigured = true;

        ImGuiIO &io = ImGui::GetIO();

        ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 320, 20));
    }

    ImGui::Begin("Inspect", nullptr, 0);

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Properties");

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Components");
    for (auto& component : system.inspectedNode->Components()) {
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", component->TypeName().c_str());
    }

    ImGui::End();
}

void DrawTree(KaijuWorldSystem& system, SP<WorldNode> node, String name) {
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

            ImGui::SameLine();

            if (ImGui::SmallButton("x")) {
                DeleteNodeCommandCore commandCore{node, node->Parent()};
                auto command = MAKE<Command<DeleteNodeCommandCore>>(String("Delete ") + node->name,
                                                                    commandCore,
                                                                    [](auto& command) { command.core.node->Destroy(); },
                                                                    [](auto& command) { command.core.node->UnDestroy(); command.core.parent->Add(command.core.node); } );
                system.commands.Run(command);
            }

            ImGui::SameLine();

            if (ImGui::SmallButton("i")) {
                system.inspectedNode = node;
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

            DrawTree(system, childNode, name);
        }
        ImGui::TreePop();
    }
}


class KaijuImGuiRenderProcessor : public RenderProcessor {
public:
    using Base = RenderProcessor;

    KaijuWorldSystem& system;
    List<SP<KaijuSceneClass>> sceneClasses;
    bool isWindowConfigured = false;
    bool isToolActive = false;

    // TODO: re-evaluate all registered render phases, probably don't want to run for each camera
    KaijuImGuiRenderProcessor(KaijuWorldSystem& system) : Base("", {RenderPhase::PostClear}), system(system) {
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
        sceneClasses.push_back(MAKE<KaijuSceneClass>("app.animate", "Test Animate App", []() { return MAKE<TestAnimateApp>(); }));
        sceneClasses.push_back(MAKE<KaijuSceneClass>("example.life", "Life Example", []() { return MAKE<ExampleLife::Scene>(); }));
    }

    // MARK: RenderProcessor

    void Process(String phase) override {
        auto world = system.World();
        GUARD(world)

        GUARD(system.isUIVisible)

        ImGUIMainMenuBarBuilder menuBuilder;
        menuBuilder.Build(system.menus);

        BuildInspectedNodeWindow(system);

        if (!system.isToolsWindowConfigured) {
            system.isToolsWindowConfigured = true;

            ImGuiIO &io = ImGui::GetIO();

            ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
            ImGui::SetNextWindowPos(ImVec2(20, 20));
        }

        ImGui::Begin("Kaiju", &isToolActive, 0);

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
                        system.commands.Clear();

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
                        auto resourceId = res.second.info.id;
                        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s, Used: %ld", resourceId.c_str(), res.second.resource.use_count());

                        if (String(name) == String("texture")) {
                            ImGui::SameLine();
                            
                            String addButtonName("+");
                            addButtonName += "##texture-";
                            addButtonName += resourceId;

                            if (ImGui::SmallButton(addButtonName.c_str())) {
                                world->And()
                                    .With<SpriteRenderer>(SCAST<SomeTexture>(res.second.resource));
                            }
                        }
                    }
                }
            }
        }

        DrawTree(system, world->root, "Root");

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Render Engine");

        static bool optimizeStateSwitches = true;
        auto renderEngine = world->renderContext->renderEngine;
        ImGui::Checkbox("Optimize State Switches", &optimizeStateSwitches);
        renderEngine->optimizeStateSwitches = optimizeStateSwitches;

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Enabled Features");
        for (auto const& enabledFeature : renderEngine->EnabledFeatures()) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", enabledFeature.c_str());
        }

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

        if (ImGui::CollapsingHeader("Command History")) {
            auto& commands = system.commands.Commands();

            if (ImGui::Button("Undo")) {
                system.commands.Undo();
            }
            ImGui::SameLine();
            if (ImGui::Button("Redo")) {
                system.commands.Redo();
            }

            for (auto& command : commands) {
                auto name = command->name;
                GUARD_CONTINUE(!IsEmpty(name));

                switch (command->State()) {
                    case SomeCommand::StateType::Complete:
                        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", name.c_str());
                        break;
                    default:
                        ImGui::TextColored(ImVec4(0.3, 0.3, 0.3, 1), "%s", name.c_str());
                        break;
                }
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
};
