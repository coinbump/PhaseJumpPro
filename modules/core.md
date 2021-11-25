# Module: Core

Classes and utilities that are common to many games.

## Class: MonoBehaviour

Common extensions to Unity's `MonoBehaviour` class.

### Methods

**RenderGizmos(EditorUtils.RenderState)**: Wraps multiple separate Unity calls into one, using a `RenderState` type.

### Stability

Stable.

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

Simple class. Stable.