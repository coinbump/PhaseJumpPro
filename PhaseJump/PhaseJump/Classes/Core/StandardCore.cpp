#include "StandardCore.h"
#include "Class.h"

using namespace PJ;

String StandardCore::ClassId() const {
    if (this->_class) {
        return this->_class->_core.id;
    }

    return this->classId;
}
