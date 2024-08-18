#ifndef PJANYFACTORY_H_
#define PJANYFACTORY_H_

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// For type erasure
    class AnyFactory {
    public:
        virtual ~AnyFactory() {}
    };
} // namespace PJ

#endif
