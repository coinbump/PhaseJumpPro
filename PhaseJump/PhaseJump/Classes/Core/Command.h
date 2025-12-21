#pragma once

#include "Dev.h"
#include "StateMachine.h"
#include "Void.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Wraps do, undo, and redo behavior. Typically a command will store a memento
    /// that allows it to reverse the state
    class SomeCommand {
    public:
        using This = SomeCommand;

        enum class StateType { Default, Complete, Reversed };

        virtual ~SomeCommand() {}

        virtual String Name() const = 0;
        virtual StateType State() const = 0;
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

    protected:
        StateMachine<StateType> stateMachine;

    public:
        Core core{};
        String name;

        Command(String name, RunFunc runFunc, UndoFunc undoFunc) :
            name(name),
            runFunc(runFunc),
            undoFunc(undoFunc) {}

        Command(String name, Core const& core, RunFunc runFunc, UndoFunc undoFunc) :
            name(name),
            core(core),
            runFunc(runFunc),
            undoFunc(undoFunc) {}

        // MARK: SomeCommand

        StateType State() const override {
            return stateMachine.State();
        }

        String Name() const override {
            return name;
        }

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
