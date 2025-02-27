#pragma once

#include "Base.h"
#include "Class.h"
#include "FactoryRegistry.h"
#include <memory>
#include <stdio.h>
#include <vector>

// Experimental
namespace PJ {
    //    static FactoryRegistry<Class> classRegistry;

    /// An example of a class with a default platform-neutral implementation
    class CoreExample {};

    /// Registers class factories to create a class object that instantiates the
    /// object for a specific behavior Example: a class that creates an object
    /// that plays an audio clip might have a Windows platform implementation
    /// and a NextStep implementation
    class SomePlatform : public Base {
    protected:
        UP<SomePlatform> parent;
        UP<SomePlatform> child;

    public:
        virtual ~SomePlatform() {}

        SomePlatform* Root() {
            auto result = this;
            auto parent = this->parent.get();

            while (nullptr != parent) {
                result = parent;
                parent = parent->parent.get();
            }

            return result;
        }

        // MARK: Base

        void OnGo() override {
            auto root = Root();
            while (nullptr != root) {
                root->Register();
                root = root->child.get();
            }
        }

        virtual void Register() = 0;
    };

    /// Platform neutral code using C++ stdlib
    class BasePlatform : public SomePlatform {
        void Register() override;
    };

    void BasePlatform::Register() {
        // Register NextStep platform classes here
    }

    class NSPlatform : public SomePlatform {
    public:
        NSPlatform();

        void Register() override;
    };

    NSPlatform::NSPlatform() {
        parent = NEW<BasePlatform>();
    }

    void NSPlatform::Register() {
        // Register NextStep platform classes here
    }

    class MacPlatform : public SomePlatform {
    public:
        MacPlatform();

        void Register() override;
    };

    MacPlatform::MacPlatform() {
        parent = NEW<NSPlatform>();
    }

    void MacPlatform::Register() {
        // Register Mac platform classes here
    }

    class iOSPlatform : public SomePlatform {
    public:
        iOSPlatform();

        void Register() override;
    };

    iOSPlatform::iOSPlatform() {
        parent = NEW<NSPlatform>();
    }

    void iOSPlatform::Register() {
        // Register iOS platform classes here
    }

    class WindowsPlatform : public SomePlatform {
    public:
        WindowsPlatform();

        void Register() override;
    };

    WindowsPlatform::WindowsPlatform() {
        parent = NEW<BasePlatform>();
    }

    void WindowsPlatform::Register() {
        // Register Windows platform classes here
    }
} // namespace PJ
