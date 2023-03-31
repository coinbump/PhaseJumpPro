#include "Core.h"
#include "Class.h"

using namespace PJ;

String Core::ClassId() const
{
    if (this->_class) {
        return this->_class->id;
    }

    return this->classId;
}
