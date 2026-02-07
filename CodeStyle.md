# Coding Style

This document establishes best practices and coding styles for this project.

## Getters and Setters

For convenience, don't use Get for getters, unless the type that the getter function would return matches the name of the function.

Good:

```cpp
int PurchaseCount() const;
```

Bad:

```cpp
World* World() const;
```

Instead:

```cpp
World* GetWorld() const;
```

## Interfaces and Some Prefix

The prefix `some` has been used to designate a purely virtual interface class.

Interface classes should not store data or state, and should not have implementations for core functionality unless the implementation is trivial.

Example:

```cpp
class SomeTool {
    virtual void Foo() = 0;
}

class Tool: public SomeTool {
    // MARK: SomeTool

    void Foo() override;
}
```
