#pragma once

#include "DiagnoseModel.h"
#include "StringConvertible.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/8/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Base class for many framework objects, provides standard behavior and
    /// provides polymorphism for factories so we can use dynamic cast and
    /// `shared_from_this`
    class Base : public StringConvertible, public std::enable_shared_from_this<Base> {
    protected:
        /// If true, the object was started by calling Go
        bool didGo = false;

        /// Called only if the object hasn't been started already
        virtual void GoInternal() {}

    public:
        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

        using RootBaseType = Base;

        virtual ~Base() {}

        /// Start the object if needed
        virtual void Go() {
            GUARD(!didGo);
            didGo = true;

            GoInternal();
        }

        /// @return Returns description for logs
        String ToString() const override {
            return "Base";
        }
    };
} // namespace PJ
