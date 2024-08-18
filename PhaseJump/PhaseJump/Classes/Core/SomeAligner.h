#ifndef PJSOMEALIGNER_H
#define PJSOMEALIGNER_H

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/8/23
 */
namespace PJ {
    /// Aligns a view inside another view
    /// This uses reading coordinates (positive Y is down)
    class SomeAligner {
    public:
        virtual ~SomeAligner() {}

        virtual float AlignedOrigin(float layoutSize, float frameSize) = 0;
    };

    // TODO: re-evaluate using funcs
    struct CenterAligner : public SomeAligner {
    public:
        float AlignedOrigin(float layoutSize, float frameSize) override {
            return layoutSize / 2.0f - frameSize / 2.0f;
        }
    };
} // namespace PJ

#endif
