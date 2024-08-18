#ifndef PJSWITCHABLE_H
#define PJSWITCHABLE_H

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 6/7/24
 */
namespace PJ {
    /// Can be turned on or off
    class Switchable {
    public:
        virtual ~Switchable() {}

        virtual bool IsOn() const = 0;
        virtual void SetIsOn(bool value) = 0;
        virtual void OnSwitchChange() = 0;
    };
} // namespace PJ

#endif
