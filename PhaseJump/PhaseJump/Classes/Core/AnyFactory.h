#ifndef PJANYFACTORY_H_
#define PJANYFACTORY_H_

/*
 RATING: 5 stars
 Simple abstract
 CODE REVIEW: 12/26/22
 */
namespace PJ {
    /// <summary>
    /// For type erasure
    /// </summary>
    class AnyFactory {
    public:
        virtual ~AnyFactory() {}
    };
}

#endif
