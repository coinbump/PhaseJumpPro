#ifndef PJSWITCHABLE_H
#define PJSWITCHABLE_H

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/18/22
 */
namespace PJ
{
    /// <summary>
    /// Can be turned on or off
    /// </summary>
    class Switchable
    {
    public:
        virtual ~Switchable() {}
        
        virtual bool IsOn() const = 0;
        virtual void SetIsOn(bool value) = 0;
        virtual void OnSwitchChange() = 0;
    };
}

#endif
