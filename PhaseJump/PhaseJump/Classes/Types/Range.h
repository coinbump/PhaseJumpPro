#ifndef PJRANGE_H_
#define PJRANGE_H_

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 11/1/22
 */
namespace PJ
{
    /// <summary>
    /// Specifies a range of values
    /// </summary>
    template <class T>
    struct Range
    {
    public:
        T minValue;
        T maxValue;
    };
}

#endif  // RANGE_H_
