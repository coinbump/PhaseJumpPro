#pragma once

#include "Dev.h"
#include "DiagnoseModel.h"
#include "StringConvertible.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Base class for many framework objects, provides standard behavior and
    /// polymorphism for factories
    class Base : public StringConvertible, public std::enable_shared_from_this<Base> {
    protected:
        /// If true, the object was started by calling Go
        bool didGo = false;

        /// Called only if the object hasn't been started already
        virtual void OnGo() {}

    public:
        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

        using RootBaseType = Base;

        virtual ~Base() {}

        /// Start the object if needed
        virtual void Go() {
            GUARD(!didGo);
            didGo = true;

            OnGo();
        }

        // MARK: StringConvertible

        /// @return Returns description for logs
        String ToString() const override {
            return "Base";
        }
    };
} // namespace PJ
