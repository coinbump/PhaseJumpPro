//
//  SomeEventHandler.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 5/4/23.
//

#ifndef SomeEventHandler_h
#define SomeEventHandler_h

#include "WorldComponent.h"
#include "SomeListener.h"

namespace PJ
{
    /// <summary>
    /// A component that listens to events
    /// </summary>
    class SomeEventHandler : public WorldComponent, public SomeListener
    {
    public:
    };
}

#endif
