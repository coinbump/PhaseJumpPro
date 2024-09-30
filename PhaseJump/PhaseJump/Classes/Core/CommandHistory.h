#pragma once

#include "Command.h"
#include "List.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Keeps track of all commands, flushes old commands
    /// and allows for undo-redo through command history
    class CommandHistory {
    public:
        using CommandList = VectorList<SP<SomeCommand>>;

    protected:
        CommandList commands;

        int32_t activeCommandIndex = -1;

    public:
        size_t maxSize = 1;

        CommandHistory(size_t maxSize) :
            maxSize(maxSize) {}

        CommandList const& Commands() const {
            return commands;
        }

        /// For unit tests
        int32_t ActiveCommandIndex() const {
            return activeCommandIndex;
        }

        size_t Size() const {
            return commands.size();
        }

        /// @return Returns the command at the specified index. Throws an error if index is invalud
        SomeCommand& At(size_t index) {
            GUARD_THROW(
                index >= 0 && index < commands.size(), std::out_of_range("Invalid command index")
            )
            return *commands[index];
        }

        void Clear() {
            commands.clear();
            activeCommandIndex = -1;
        }

        /// Runs the command and adds it to the command list
        void Run(SP<SomeCommand> command) {
            GUARD(command)
            GUARD(maxSize > 0)

            /// Push back the commands and erase the old ones
            if (activeCommandIndex + 1 >= maxSize) {
                commands.erase(commands.begin());
                activeCommandIndex--;
            }

            activeCommandIndex++;

            /// We can't add here because we might be in the middle of the command history
            commands.resize(activeCommandIndex + 1);
            commands[activeCommandIndex] = command;

            command->Run();
        }

        String UndoCommandName() {
            GUARDR(CanUndo(), "")
            return commands[activeCommandIndex]->name;
        }

        String RedoCommandName() {
            GUARDR(CanRedo(), "")
            return commands[activeCommandIndex + 1]->name;
        }

        void Undo() {
            GUARD(CanUndo())
            commands[activeCommandIndex]->Undo();
            activeCommandIndex--;
        }

        bool CanUndo() const {
            return activeCommandIndex >= 0;
        }

        void Redo() {
            GUARD(CanRedo())

            auto redoCommandIndex = activeCommandIndex + 1;
            commands[redoCommandIndex]->Run();
            activeCommandIndex = redoCommandIndex;
        }

        bool CanRedo() {
            auto redoCommandIndex = activeCommandIndex + 1;
            return redoCommandIndex >= 0 && redoCommandIndex < commands.size();
        }
    };
} // namespace PJ
