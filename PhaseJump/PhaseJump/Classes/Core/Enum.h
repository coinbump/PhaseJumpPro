#ifndef PJENUM_H
#define PJENUM_H

#include "Core.h"
#include "EnumClass.h"
#include "_String.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Uses EnumClass to map enum <-> String/name for serialization and display
    template <typename EnumType>
    class Enum : public Core {
    public:
        EnumType value = EnumType();

        Enum(SP<EnumClass<EnumType>> _class) {
            this->_class = _class;
        }

        virtual ~Enum() {}

        SP<EnumClass<EnumType>> EnumClass() const { return SCAST<PJ::EnumClass<EnumType>>(_class); }

        virtual String Id() const { return EnumClass()->IdOf(value); }
        virtual String Name() const { return EnumClass()->NameOf(value); }
        virtual size_t Index() const { return EnumClass()->IndexOf(value); }
    };
}

#endif
