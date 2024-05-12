#ifndef PJSOMEDEPENDENCYRESOLVER_H
#define PJSOMEDEPENDENCYRESOLVER_H

namespace PJ {
    /// <summary>
    /// Hides the use of singletons vs dependency injection
    /// </summary>
    template <class T>
    class SomeDependencyResolver
    {
    public:
        virtual ~SomeDependencyResolver() {}
        
        virtual T Dependency() = 0;
    };
}

#endif
