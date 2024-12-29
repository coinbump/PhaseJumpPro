#pragma once

#include "Base.h"
#include "Class.h"
#include "StringUtils.h"
#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/27/24
 */
namespace PJ {
    /// Standard composable core with common properties
    /// Or you can define your own
    class StandardCore {
    public:
        // TODO: SP-Audit
        using ClassPtr = SP<Class<>>;

    protected:
        /// Class object that defines metadata properties for this object
        ClassPtr _class;

        /// Class id to find the class if it hasn't been assigned
        String classId;

    public:
        /// Unique identifier
        String id;

        /// User facing name for display
        String name;

        /// Custom properties
        Tags tags;

        /// Object attribute types
        TypeTagSet typeTags;

        StandardCore() {}

        /// @return Returns the id of the class object attached, or if there is no class object,
        /// returns the stored class id
        String ClassId() const;

        void SetClassId(String value) {
            classId = value;
        }

        ClassPtr Class() const {
            return _class;
        }

        void SetClass(ClassPtr _class) {
            this->_class = _class;
        }
    };
} // namespace PJ
