#ifndef PJRESOURCETYPEENUM_H
#define PJRESOURCETYPEENUM_H

#include "Enum.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    /// Resource types recognized by ResourceRepository
    enum class ResourceType { Texture };

    class ResourceTypeEnumClass : public EnumClass<ResourceType> {
    public:
        using Base = EnumClass<ResourceType>;

        ResourceTypeEnumClass() :
            Base("enum.resourceType") {
            MapEnum(ResourceType::Texture, "texture", "Texture");
        }
    };

    class ResourceTypeEnum : public Enum<ResourceType> {
    public:
        using Base = Enum<ResourceType>;

        static const SP<ResourceTypeEnumClass> enumClass;

        ResourceTypeEnum() :
            Base(enumClass) {}
    };
} // namespace PJ

#endif
