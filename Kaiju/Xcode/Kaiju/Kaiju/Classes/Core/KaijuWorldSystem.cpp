#include "KaijuWorldSystem.h"
#include "ExampleCheckersScene.h"
#include "ExampleLifeScene.h"
#include "ExampleMatrixBoardViewAnimationScene.h"
#include "FilesProcessorScene.h"
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
#include "TestZOrderScene.h"

using namespace std;
using namespace PJ;

KaijuWorldSystem::KaijuWorldSystem() {
    // auto menuItem = MAKE<MenuItem>("Hello", VectorList<KeyboardShortcut>{{"c",
    // {EventModifier::Shift, EventModifier::Control}}, {"A", {EventModifier::OptionAlt}}},
    // [](auto& menuItem) { std::cout << "Hello" << std::endl; }); menuItem->isToggleOn = true;
    typeTags.insert(EditorTypeTag::Persist);

    auto undoMenuItem = MAKE<MenuItem>(
        "Undo", VectorList<KeyboardShortcut>{ { "z", { KeyModifier::Shortcut } } },
        [this](auto& menuItem) { this->commands.Undo(); }
    );
    auto redoMenuItem = MAKE<MenuItem>(
        "Redo",
        VectorList<KeyboardShortcut>{ { "z", { KeyModifier::Shortcut, KeyModifier::Shift } } },
        [this](auto& menuItem) { this->commands.Redo(); }
    );

    undoMenuItem->titleFunc = [this](auto& menuItem) {
        return Joined({ "Undo", commands.UndoCommandName() }, " ");
    };
    undoMenuItem->isEnabledFunc = [this](auto& menuItem) { return commands.CanUndo(); };
    redoMenuItem->titleFunc = [this](auto& menuItem) {
        return Joined({ "Redo", commands.RedoCommandName() }, " ");
    };
    redoMenuItem->isEnabledFunc = [this](auto& menuItem) { return commands.CanRedo(); };

    Menu::MenuItemList menuItems{ undoMenuItem, redoMenuItem, MAKE<SeparatorMenuItem>() };
    auto menu = NEW<Menu>("Edit", menuItems);
    menus.push_back(std::move(menu));

    RegisterSceneClasses();
}

void KaijuWorldSystem::RegisterSceneClasses() {
    auto testTextureSceneClass =
        NEW<SceneClass>("test.texture", "Test Texture", []() { return MAKE<TestTextureScene>(); });
    sceneClasses.Add(testTextureSceneClass);
    sceneClasses.Add(NEW<SceneClass>("test.slicedTexture", "Test Sliced Texture", []() {
        return MAKE<TestSlicedTextureScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.gradients", "Test Gradients", []() {
        return MAKE<TestGradientsScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.meshPath", "Test Mesh Path", []() {
        return MAKE<TestMeshPathScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.audio", "Test Audio", []() {
        return MAKE<TestAudioScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.emitters", "Test Emitters", []() {
        return MAKE<TestEmittersScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.zOrder", "Test ZOrder", []() {
        return MAKE<TestZOrderScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.editBezier", "Test edit bezier", []() {
        return MAKE<TestEditBezierScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.theme", "Test theme", []() {
        return MAKE<TestThemeScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.ui", "Test UI", []() { return MAKE<TestUIScene>(); }));
    sceneClasses.Add(NEW<SceneClass>("test.steering", "Test steering", []() {
        return MAKE<TestSteeringScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("test.bullets", "Test bullets", []() {
        return MAKE<TestBulletsScene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("app.animate", "Test animate app", []() {
        return MAKE<TestAnimateApp>();
    }));
    sceneClasses.Add(NEW<SceneClass>("example.life", "Example: Life", []() {
        return MAKE<Example::Life::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>(
        "example.matrixBoardViewAnimation", "Example: Matrix board view",
        []() { return MAKE<Example::MatrixBoardViewAnimation::Scene>(); }
    ));
    sceneClasses.Add(NEW<SceneClass>("example.checkers", "Example: Checkers", []() {
        return MAKE<Example::Checkers::Scene>();
    }));
    sceneClasses.Add(NEW<SceneClass>("app.filesProcessor", "Files processor", []() {
        return MAKE<Kaiju::FilesProcessorScene>();
    }));
}

void KaijuWorldSystem::ProcessUIEvents(UIEventList const& uiEvents) {
    for (auto& event : uiEvents) {
        auto keyDownEvent = DCAST<KeyDownUIEvent>(event);
        if (keyDownEvent) {
            switch (keyDownEvent->keyCode.value) {
            case '\t':
                isUIVisible = !isUIVisible;
                break;
            }
        }
    }
}
