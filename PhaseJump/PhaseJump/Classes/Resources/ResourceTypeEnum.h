#ifndef PJRESOURCETYPEENUM_H
#define PJRESOURCETYPEENUM_H

#include "Enum.h"

/*
 RATING: 5 stars
 Utility type
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Resource types recognized by ResourceRepository
    enum class ResourceType {
        Texture
    };

    class ResourceTypeEnumClass : public EnumClass<ResourceType> {
    public:
        using Base = EnumClass<ResourceType>;

        ResourceTypeEnumClass() : Base("enum.resourceType") {
            MapEnum(ResourceType::Texture, "texture", "Texture");
        }
    };

    class ResourceTypeEnum : public Enum<ResourceType> {
    public:
        using Base = Enum<ResourceType>;

        static const SP<ResourceTypeEnumClass> enumClass;

        ResourceTypeEnum() : Base(enumClass) {
        }
    };
}

#endif
