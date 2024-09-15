#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

class KaijuImGuiRenderProcessor;

class KaijuWorldSystem : public SomeWorldSystem {
public:
    bool isUIVisible = true;
    bool isToolsWindowConfigured = false;
    bool isInspectWindowConfigured = false;

    CommandHistory commands;
    SP<WorldNode> inspectedNode;

    // FUTURE: Each scene can have its own menus
    /// Default menus for app, independent of current scene
    VectorList<SP<Menu>> defaultMenus;

    /// Current menus
    VectorList<SP<Menu>> menus;

    auto static constexpr commandHistorySize = 40;

    KaijuWorldSystem() : commands(commandHistorySize) {
        // auto menuItem = MAKE<MenuItem>("Hello", VectorList<KeyboardShortcut>{{"c", {EventModifier::Shift, EventModifier::Control}}, {"A", {EventModifier::OptionAlt}}}, [](auto& menuItem) { std::cout << "Hello" << std::endl; });
        // menuItem->isToggleOn = true;

        auto undoMenuItem = MAKE<MenuItem>("Undo", VectorList<KeyboardShortcut>{{"z", {EventModifier::CommandControl}}}, [this](auto& menuItem) { this->commands.Undo(); });
        auto redoMenuItem = MAKE<MenuItem>("Redo", VectorList<KeyboardShortcut>{{"z", {EventModifier::CommandControl, EventModifier::Shift}}}, [this](auto& menuItem) { this->commands.Redo(); });

        undoMenuItem->titleFunc = [this](auto& menuItem) {
            return Joined({"Undo", commands.UndoCommandName()}, " ");
        };
        undoMenuItem->isEnabledFunc = [this](auto& menuItem) {
            return commands.CanUndo();
        };
        redoMenuItem->titleFunc = [this](auto& menuItem) {
            return Joined({"Redo", commands.RedoCommandName()}, " ");
        };
        redoMenuItem->isEnabledFunc = [this](auto& menuItem) {
            return commands.CanRedo();
        };

        Menu::MenuItemList menuItems{undoMenuItem, redoMenuItem, MAKE<SeparatorMenuItem>()};
        auto menu = MAKE<Menu>("Edit", menuItems);
        menus.push_back(menu);
    }

    // MARK: SomeWorldSystem
    void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override {
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
};
