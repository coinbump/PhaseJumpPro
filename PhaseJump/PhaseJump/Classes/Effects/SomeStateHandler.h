#ifndef PJSOMESTATEHANDLER_H
#define PJSOMESTATEHANDLER_H

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 5/8/23
 */
namespace PJ
{
    /// <summary>
    /// Handles state changes
    /// </summary>
    template <class StateType>
    class SomeStateHandler : public WorldComponent
    {
    public:
        virtual void OnStateChange(StateType state) = 0;
    };
}

#endif
