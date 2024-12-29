#include "KaijuWorldSystem.h"
#include "ExampleCheckersScene.h"
#include "ExampleGraphsScene.h"
#include "ExampleLifeScene.h"
#include "ExampleMatrixBoardViewAnimationScene.h"
#include "ExampleMinesweeperScene.h"
#include "FilesProcessorScene.h"
#include "ImageAppScene.h"
#include "TestAnimateApp.h"
#include "TestAudioScene.h"
#include "TestBulletsScene.h"
#include "TestEditBezierScene.h"
#include "TestEmittersScene.h"
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestSlicedTextureScene.h"
#include "TestSteeringScene.h"
#include "TestTextureScene.h"
#include "TestThemeScene.h"
#include "TestUIScene.h"
#include "TestViewsScene.h"
#include "TestZOrderScene.h"

using namespace std;
using namespace PJ;

KaijuWorldSystem::KaijuWorldSystem() {
    // auto menuItem = MAKE<MenuItem>("Hello", VectorList<KeyboardShortcut>{{"c",
    // {EventModifier::Shift, EventModifier::Control}}, {"A", {EventModifier::OptionAlt}}},
    // [](auto& menuItem) { std::cout << "Hello" << std::endl; }); menuItem->isToggleOn = true;
    typeTags.insert(EditorTypeTag::Persist);

    auto undoMenuItem = NEW<MenuItem>(
        "Undo", VectorList<KeyboardShortcut>{ { "z", { KeyModifier::Shortcut } } },
        [this](auto& menuItem) { commands.Undo(); }
    );
    auto redoMenuItem = NEW<MenuItem>(
        "Redo",
        VectorList<KeyboardShortcut>{ { "z", { KeyModifier::Shortcut, KeyModifier::Shift } } },
        [this](auto& menuItem) { commands.Redo(); }
    );

    undoMenuItem->titleFunc = [this](auto& menuItem) {
        return Joined({ "Undo", commands.UndoCommandName() }, " ");
    };
    undoMenuItem->isEnabledFunc = [this](auto& menuItem) { return commands.CanUndo(); };
    redoMenuItem->titleFunc = [this](auto& menuItem) {
        return Joined({ "Redo", commands.RedoCommandName() }, " ");
    };
    redoMenuItem->isEnabledFunc = [this](auto& menuItem) { return commands.CanRedo(); };

    auto menu = NEW<Menu>("Edit");
    menu->items.push_back(std::move(undoMenuItem));
    menu->items.push_back(std::move(redoMenuItem));
    menu->items.push_back(NEW<SeparatorMenuItem>());
    menus.push_back(std::move(menu));

    RegisterSceneClasses();
}

void KaijuWorldSystem::RegisterSceneClasses() {
    auto testTextureSceneClass =
        NEW<SceneClass>("test.texture", "Test Texture", []() { return NEW<TestTextureScene>(); });
    sceneClasses.Add(testTextureSceneClass);
    sceneClasses.Add(NEW<SceneClass>("test.slicedTexture", "Test Sliced Texture", []() {
        return NEW<TestSlicedTextureScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.gradients", "Test Gradients", []() {
        return NEW<TestGradientsScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.meshPath", "Test Mesh Path", []() {
        return NEW<TestMeshPathScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.audio", "Test Audio", []() {
        return NEW<TestAudioScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.emitters", "Test Emitters", []() {
        return NEW<TestEmittersScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.zOrder", "Test ZOrder", []() {
        return NEW<TestZOrderScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.editBezier", "Test edit bezier", []() {
        return NEW<TestEditBezierScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.theme", "Test theme", []() {
        return NEW<TestThemeScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.ui", "Test UI", []() { return NEW<TestUIScene>(); }));
    sceneClasses.Add(NEW<SceneClass>("test.steering", "Test steering", []() {
        return NEW<TestSteeringScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.bullets", "Test bullets", []() {
        return NEW<TestBulletsScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.views", "Test views", []() {
        return NEW<TestViewsScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("app.animate", "Test animate app", []() {
        return NEW<TestAnimateApp>();
    }));
    sceneClasses.Add(NEW<SceneClass>("example.life", "Example: Life", []() {
        return NEW<Example::Life::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>(
        "example.matrixBoardViewAnimation", "Example: Matrix board view",
        []() { return NEW<Example::MatrixBoardViewAnimation::Scene>(); }
    ));
    sceneClasses.Add(NEW<SceneClass>("example.mineSweeper", "Example: Minesweeper", []() {
        return NEW<Example::Minesweeper::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("example.graphs", "Example: Graphs", []() {
        return NEW<Example::Graphs::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("example.checkers", "Example: Checkers", []() {
        return NEW<Example::Checkers::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("app.filesProcessor", "Files processor", []() {
        return NEW<Kaiju::FilesProcessorScene>();
    }));

    // MARK: Apps

    sceneClasses.Add(NEW<SceneClass>("app.image", "App: Image", []() {
        return NEW<ImageAppScene>();
    }));
}

void KaijuWorldSystem::ProcessUIEvents(UIEventList const& uiEvents) {
    for (auto& event : uiEvents) {
        auto keyDownEvent = DCAST<KeyDownUIEvent>(event);
        if (keyDownEvent) {
            switch (keyDownEvent->keyCode.value) {
            case SDLK_TAB:
                if (keyDownEvent->keyModifiers.Contains(KeyModifier::Control)) {
                    isUIVisible = !isUIVisible;
                }
                break;
            }
        }
    }
}
