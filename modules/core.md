# Module: Core

Classes and utilities that are common to many games.

## Class: Base

### Description

Provides common interface for all objects.

_Example:_ Go() is a common interface for game objects that need to be kickstarted in some way.

### Stability

Simple class. Stable.


## Class: CircleSpawner2D

### Description

Spawns a game object inside a 2D circle determined by its radius. The spawn area extends from the spawner's position, which defines the center of the circle.

### Stability

Stable.


## Class: MonoBehaviour

### Description

Common extensions to Unity's `MonoBehaviour` class.

### Methods

**RenderGizmos(EditorUtils.RenderState)**: Wraps multiple separate Unity calls into one, using a `RenderState` type.

### Stability

Stable.

## Class: Node2D

[Node2D](core/Node2D.md)

## Class: RandomUtils

### Description

Utilities for random choices & behavior.

### Methods

**ChooseFrom(Type[])**: Returns a random element from the array, or a `default` value if the array is empty.

**ChooseFrom(List)**: Returns a random element from the list, or a `default` value if the array is empty.

**VaryFloat(float value, float vary)**: Returns the original float value + a random delta.

_Example_: `float(10, 3)` produces random values between 7-13.

**Delta(float delta)**: Returns a random value between -delta...delta

### Stability

Simple utilities. Stable.


## Class: RectSpawner2D

### Description

Spawns a game object inside a 2D rectangle determined by width, height. The spawn area extends from the spawner's position, which defines the center of the rectangle.

### Stability

Stable.
