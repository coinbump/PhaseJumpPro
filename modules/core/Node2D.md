## Class: Node2D

### Description

The core class for all 2D game objects in a Phase Jump project. Provides built-in common design pattern support for tags (custom properties attached to object), and state machines.

## Public Properties

**CullType cullType**: Determines how this object is culled, if at all.

**Bool dontModRotation**: _Default: false_. If true, don't mod rotation values between 0-360.0f and 0-1.0f.

**bool isKinematic**: Cached value, determined by rigidbody.

**Tags tags**: Container for custom properties.

**float RotationDegreeAngle**: Value of 0-360.0f for the node's rotation degree angle.

**float RotationNormal**: Value of 0-1.0f for the node's normalized rotation degree angle.

_NOTE:_ if property `dontModRotation` is true, this value can exceed the normalized float value range.

### Public Methods

**EvtStateChanged(SomeStateMachine)**: Called when the built in state machine changes state. Subclasses should override this method.

**EvtStateFinished(SomeStateMachine)**: Called when a state machine state timer finishes. Subclasses can override this method.

_Example:_ A state that lasts for N seconds.

**HasTag(string name)**: Returns true if either a GameObject tag, or a PhaseJump tag exists with a matching name.

_Note:_ GameObject tags are defined in the Unity editor. PhaseJump tags are stored in the `tags` property inside `Node2D`.


### Internal Methods

**OnBecameInvisible**: Optionally culls the object when it leaves the visible camera space, depending on the `cullType` property.

### Internal Properties

**Core core**: Propagates state machine message to the Node2D owner.

**StateMachine\<string> state**: Default state machine for all Node2D objects. Uses `string` instead of a concrete type for maximum flexibility.

_Note:_ Subclasses of Node2D can implement their own state machines with concrete types as needed, but generally strings are easier to deal with.

### Stability

Changes often to improve the 2D node workflow.
