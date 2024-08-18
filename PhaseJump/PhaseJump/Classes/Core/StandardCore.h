#pragma once

#include "_String.h"
#include "Base.h"
#include "Class.h"
#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/10/24
 */
namespace PJ {
    /// Standard composable core with common properties
    /// Or you can define your own
    class StandardCore {
    public:
        using ClassPtr = SP<Class<>>;

    protected:
        ClassPtr _class;

        /// Used if class is not yet assigned
        String classId;

    public:
        /// Custom properties
        Tags tags;

        StandardCore(ClassPtr _class = nullptr, String classId = "") :
            _class(_class),
            classId(classId) {}

        /// If we have a class object, return its id, otherwise use the id we
        /// have
        String ClassId() const;

        void SetClassId(String value) {
            this->classId = value;
        }

        ClassPtr Class() const {
            return _class;
        }

        void SetClass(ClassPtr _class) {
            this->_class = _class;
        }
    };
} // namespace PJ
