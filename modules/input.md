# Module: Input

Utilities for dealing with user input: Mouse, Joystick, Keyboard, etc.

## Class: AimAtMouseInputControlScheme2D

### Description

On every update cycle, rotates the GameObject to point at the mouse position.

### Requirements

The GameObject must have a `Node2D` script attached.

### Stability

Stable.


## Interface: SomeInputController

### Methods

**bool IsAvailable()**: Returns true if this input controller is available.

_Example:_ returns false for mouse input when using a joystick.

## Interface: SomeInputControlScheme

### Description

Empty abstract interface class.

## Class: MouseInputController

### Description

On every update cycle, records the mouse position in screen space and the Ray that intersects with the mouse position in game space.

### Stability

Simple class. Stable.

## Class: MoveToMouseInputControlScheme2D

### Description

On every update cycle, moves the attached GameObject to the mouse position (kinematic objects only).

### Stability

Stable.
