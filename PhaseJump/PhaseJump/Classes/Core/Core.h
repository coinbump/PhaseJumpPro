#ifndef PJCORE_H
#define PJCORE_H

#include "Base.h"
#include "Class.h"
#include "_String.h"
#include "Tags.h"
#include <memory>

namespace PJ
{
    // Extends Base with common patterns
    class Core : public Base {
    protected:
        std::shared_ptr<Class> _class;
        String classId;   // If class is not yet assigned

    public:
        /// <summary>
        /// If we have a class object, return its id, otherwise use the id we have
        /// </summary>
        String ClassId() const
        {
            if (this->_class) {
                return this->_class->id;
            }

            return this->classId;
        }

        void SetClassId(String value)
        {
            this->classId = value;
        }

        /// <summary>
        /// Custom properties
        /// </summary>
        std::shared_ptr<Tags> tags = std::make_shared<Tags>();
    };
}

#endif
