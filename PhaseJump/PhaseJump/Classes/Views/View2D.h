#ifndef PJVIEW2D_H
#define PJVIEW2D_H

#include "Node2D.h"
#include "SomeUIEvent.h"
#include <optional>

namespace PJ {
    class View2D : public Node2D, public SomePointerEventsResponder {
    public:
        virtual std::optional<float> IntrinsicHeight() { return std::nullopt; }
    };
}

#endif
