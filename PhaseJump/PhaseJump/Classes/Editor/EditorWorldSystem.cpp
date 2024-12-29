#include "EditorWorldSystem.h"
#include "CollectionUtils.h"
#include "EditorTypes.h"
#include "SignalTypes.h"
#include "SomeKeyUIEvent.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

/// Core for add node command
struct AddNodeCommandCore {
    SP<WorldNode> node;
    WorldNode* parent{};
    size_t index{};
};

/// Core for delete node command
struct DeleteNodeCommandCore {
    SP<WorldNode> node;
    WorldNode* parent{};
    size_t index{};
};

EditorWorldSystem::EditorWorldSystem(int commandHistorySize) :
    commands(commandHistorySize) {

    typeTags.insert(EditorTypeTag::Persist);

    AddKeyFunc('z', { KeyModifier::Shortcut }, [this](auto& system) { commands.Undo(); });
    AddKeyFunc('z', { KeyModifier::Shortcut, KeyModifier::Shift }, [this](auto& system) {
        commands.Redo();
    });
}

void EditorWorldSystem::Add(SP<WorldNode> node, WorldNode* parent, size_t index) {
    GUARD(parent)

    index = std::min(parent->Children().size() - 1, index);

    AddNodeCommandCore commandCore{ .node = node, .parent = parent, .index = index };
    UP<SomeCommand> command = NEW<Command<AddNodeCommandCore>>(
        String("Add ") + node->Name(), commandCore,
        [](auto& command) {
            command.core.node->Restore();
            command.core.parent->Insert(command.core.node, command.core.index);
        },
        [](auto& command) { command.core.node->Destroy(); }
    );
    commands.Run(command);
}

void EditorWorldSystem::Delete(SP<WorldNode> node) {
    auto parent = node->Parent();
    GUARD(parent)

    auto index = IndexOf(parent->Children(), node);
    GUARD(index)

    DeleteNodeCommandCore commandCore{ .node = node, .parent = parent, .index = *index };
    UP<SomeCommand> command = NEW<Command<DeleteNodeCommandCore>>(
        String("Delete ") + node->Name(), commandCore,
        [](auto& command) { command.core.node->Destroy(); },
        [](auto& command) {
            command.core.node->Restore();
            command.core.parent->Insert(command.core.node, command.core.index);
        }
    );
    commands.Run(command);
}
