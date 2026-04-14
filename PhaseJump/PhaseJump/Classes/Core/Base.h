#pragma once

#include "Dev.h"
#include "DiagnoseModel.h"
#include "StringConvertible.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/24/25
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Base class for many framework objects, provides standard behavior and
    /// polymorphism for factories
    class Base : public StringConvertible, public std::enable_shared_from_this<Base> {
        // FUTURE: evaluate the need for enable_shared_from_this
    protected:
        /// If true, the object was started by calling Go
        bool didGo{};

        /// Called only if the object hasn't been started already
        virtual void OnGo() {}

    public:
        using RootBaseType = Base;

        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

        virtual ~Base() {}

        /// Start the object if needed
        void Go() {
            GUARD(!didGo);
            didGo = true;

            OnGo();
        }

        bool DidGo() const {
            return didGo;
        }

        // MARK: StringConvertible

        /// @return Returns description for logs
        String ToString() const override {
            return "Base";
        }
    };
} // namespace PJ
