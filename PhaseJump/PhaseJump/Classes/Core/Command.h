#pragma once

#include "Dev.h"
#include "StateMachine.h"
#include "Void.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Wraps do, undo, and redo behavior. Typically a command will store a memento
    /// that allows it to reverse the state
    class SomeCommand {
    public:
        using This = SomeCommand;

        enum class StateType { Default, Complete, Reversed };

    protected:
        StateMachine<StateType> stateMachine;

    public:
        String name;

        SomeCommand(String name) :
            name(name) {}

        StateType State() const {
            return stateMachine.State();
        }

        virtual void Run() = 0;
        virtual void Undo() = 0;
    };

    template <class Core = Void>
    class Command : public SomeCommand {
    public:
        using This = Command<Core>;
        using Base = SomeCommand;

        using RunFunc = std::function<void(This&)>;
        using UndoFunc = std::function<void(This&)>;

        RunFunc runFunc;
        UndoFunc undoFunc;

        Core core{};

        Command(String name, RunFunc runFunc, UndoFunc undoFunc) :
            Base(name),
            runFunc(runFunc),
            undoFunc(undoFunc) {}

        Command(String name, Core const& core, RunFunc runFunc, UndoFunc undoFunc) :
            Base(name),
            core(core),
            runFunc(runFunc),
            undoFunc(undoFunc) {}

        // MARK: SomeCommand

        void Run() override {
            switch (State()) {
            case StateType::Default:
            case StateType::Reversed:
                stateMachine.SetState(StateType::Complete);
                if (runFunc) {
                    runFunc(*this);
                }
                break;
            case StateType::Complete:
                PJ::Log("ERROR. Run called twice for command.");
                break;
            default:
                break;
            }
        }

        void Undo() override {
            GUARD(undoFunc)
            GUARD(StateType::Complete == stateMachine.State())

            undoFunc(*this);
            stateMachine.SetState(StateType::Reversed);
        }
    };
} // namespace PJ
