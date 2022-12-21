#ifndef PJANIMATABLEVALUE_H
#define PJANIMATABLEVALUE_H

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// Interpolates a value from 0-1 progress
    /// </summary>
    template <class T> class SomeValueInterpolator
    {
    public:
        virtual T ValueAt(T start, T end, float progress) const = 0;
    };

    template <class T> class ValueInterpolator : public SomeValueInterpolator<T>
    {
    public:
        T ValueAt(T start, T end, float progress) const override {
            return start + (end - start) * progress;
        }
    };

    using FloatValueIntrpolator = ValueInterpolator<float>;
}

#endif
