#ifndef PJCORE_H
#define PJCORE_H

#include "Base.h"
#include "_String.h"
#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Utility object
 CODE REVIEW: 5/11/24
 */
namespace PJ
{
    class Class;
    
    class Core {
    public:
        using ClassPtr = SP<Class>;

    protected:
        ClassPtr _class;
        String classId;   // If class is not yet assigned

    public:
        /// <summary>
        /// Custom properties
        /// </summary>
        Tags tags;

        Core(ClassPtr _class = nullptr, String classId = "") : _class(_class), classId(classId) {
        }

        /// <summary>
        /// If we have a class object, return its id, otherwise use the id we have
        /// </summary>
        String ClassId() const;

        void SetClassId(String value)
        {
            this->classId = value;
        }

        ClassPtr Class() const {
            return _class;
        }

        void SetClass(ClassPtr _class) {
            this->_class = _class;
        }
    };
}

#endif
