#pragma once

#include "Macros.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    /// Protocol for a cancellable object
    class SomeCancellable {
    public:
        virtual ~SomeCancellable() {}

        virtual void Cancel() = 0;
        virtual bool IsCancelled() const = 0;
    };

    /// Implementation of cancellable object
    class Cancellable : public SomeCancellable {
    private:
        bool isCancelled = false;

    public:
        void Cancel() override {
            isCancelled = true;
        }

        bool IsCancelled() const override {
            return isCancelled;
        }
    };
}
