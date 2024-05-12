#ifndef PJSOMEALIGNER_H
#define PJSOMEALIGNER_H

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Aligns a view inside another view
    /// This uses reading coordinates (positive Y is down)
    /// </summary>
    class SomeAligner
    {
    public:
        virtual ~SomeAligner() {}
        
        virtual float AlignedOrigin(float layoutSize, float frameSize) = 0;
    };

    struct CenterAligner : public SomeAligner
    {
    public:
        float AlignedOrigin(float layoutSize, float frameSize) override
        {
            return layoutSize / 2.0f - frameSize / 2.0f;
        }
    };
}

#endif
