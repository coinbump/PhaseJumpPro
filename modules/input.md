# Module: Input

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
