//#ifndef PJSOMECOMMAND_H
//#define PJSOMECOMMAND_H
//
//namespace PJ
//{
//    /// <summary>
//    /// Commands are reversible. Typically a command will store a memento that allows it to reverse
//    /// the state
//    /// </summary>
//    class SomeCommand
//    {
//    public:
//        enum class StateType {
//            Default,
//            Complete,
//            Reversed
//        };
//
//    protected:
//        virtual void OnUndo()= 0;
//        virtual void OnExecute(bool redo) = 0;
//
//    public:
//        StateMachine<StateType> stateMachine = new StateMachine<StateType>();
//
//        StateType State => stateMachine.State;
//
//        virtual void Execute()
//        {
//            switch (State)
//            {
//                case StateType.Default:
//                    OnExecute(false);
//                    stateMachine.State = StateType.Complete;
//                    break;
//                case StateType.Reversed:
//                    OnExecute(true);
//                    stateMachine.State = StateType.Complete;
//                    break;
//                case StateType.Complete:
//                    Debug.Log("ERROR. Execute called twice for command.");
//                    break;
//                default:
//                    break;
//            }
//        }
//
//        void Undo()
//        {
//            if (StateType.Complete == stateMachine.State) {
//                OnUndo();
//                stateMachine.State = StateType.Reversed;
//            }
//        }
//    };
//}
//
//#endif
