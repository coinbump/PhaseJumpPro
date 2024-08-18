#pragma once

#include "Utils.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeListener;

    class SomeBroadcaster {
    public:
        virtual ~SomeBroadcaster() {}

        virtual void AddListener(WP<SomeListener> listener) = 0;
        virtual void RemoveListener(WP<SomeListener> listener) = 0;
    };
} // namespace PJ
