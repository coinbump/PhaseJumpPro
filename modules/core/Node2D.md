## Class: Node2D

### Description

The core class for all 2D game objects in a Phase Jump project. Provides built-in common design pattern support for tags (custom properties attached to object), and state machines.

### Methods

**HasTag(string name)**: Returns true if either a GameObject tag, or a PhaseJump tag exists with a matching name.

**EvtStateChanged(SomeStateMachine)**: Called when the built in state machine changes state. Subclasses should override this method.

**EvtStateFinished(SomeStateMachine)**: Called when a state machine state timer finishes. Subclasses can override this method.

_Example:_ A state that lasts for N seconds.

### Stability

Changes often to improve the 2D node workflow.
