#pragma once

#include "StringUtils.h"
#include "Utils.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    class Base;

    class SomeEvent {
    public:
        virtual ~SomeEvent() {}
    };

    template <class Core = Void>
    class Event : public SomeEvent {
    public:
        Core core;

        Event(Core core) :
            core(core) {}
    };

} // namespace PJ
