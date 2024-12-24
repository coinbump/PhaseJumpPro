#pragma once

#include "EnumClass.h"
#include "StandardCore.h"
#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// Uses EnumClass to map enum <-> String/name for serialization and display
    template <typename EnumType>
    class Enum : public Base {
    public:
        StandardCore core;

        using EnumClassType = EnumClass<EnumType>;

        EnumType value = EnumType();

        // TODO: SP-Audit
        Enum(SP<EnumClassType> _class) {
            core.SetClass(_class);
        }

        virtual ~Enum() {}

        SP<EnumClassType> EnumClass() const {
            return SCAST<EnumClassType>(core.Class());
        }

        virtual String Id() const {
            return EnumClass()->Id(value);
        }

        virtual String Name() const {
            return EnumClass()->Name(value);
        }

        virtual std::optional<size_t> Index() const {
            return EnumClass()->IndexOf(value);
        }
    };
} // namespace PJ
