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
    class Enum : public Base {
    public:
        using EnumClassType = EnumClass<EnumType>;

        SP<Core> core = MAKE<Core>();

        EnumType value = EnumType();

        Enum(SP<EnumClassType> _class) {
            this->core->SetClass(_class);
        }

        virtual ~Enum() {}

        SP<EnumClassType> EnumClass() const { return SCAST<EnumClassType>(core->Class()); }

        virtual String Id() const { return EnumClass()->IdOf(value); }
        virtual String Name() const { return EnumClass()->NameOf(value); }
        virtual size_t Index() const { return EnumClass()->IndexOf(value); }
    };
}

#endif
