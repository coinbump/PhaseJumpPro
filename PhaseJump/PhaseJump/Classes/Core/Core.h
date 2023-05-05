#ifndef PJCORE_H
#define PJCORE_H

#include "Base.h"
#include "_String.h"
#include "Tags.h"
#include <memory>

namespace PJ
{
    class Class;
    
    // Extends Base with common patterns
    class Core : public Base {
    protected:
        SP<Class> _class;
        String classId;   // If class is not yet assigned

    public:
        /// <summary>
        /// If we have a class object, return its id, otherwise use the id we have
        /// </summary>
        String ClassId() const;

        void SetClassId(String value)
        {
            this->classId = value;
        }

        /// <summary>
        /// Custom properties
        /// </summary>
        Tags tags;
    };
}

#endif
