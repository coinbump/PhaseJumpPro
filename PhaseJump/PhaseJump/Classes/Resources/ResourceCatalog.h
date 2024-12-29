#pragma once

#include "Base.h"
#include "Catalog.h"
#include "FilePath.h"
#include "Font.h"
#include "ResourceInfo.h"
#include "ResourceTypes.h"
#include "SomeShaderProgram.h"
#include "SomeTexture.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    class SomeTexture;
    class Font;
    class SomeShaderProgram;

    /// Bundles resource and resource info
    struct ResourceModel {
        /// The resource
        SP<Base> resource;

        ResourceInfo info;
    };

    /// A collection of resource objects, grouped by type
    class ResourceCatalog {
    public:
        Catalog<ResourceModel> value;

        void Add(ResourceModel const& model) {
            value.Add(model.info.type, model.info.id, model);
        }

        Catalog<ResourceModel>::CatalogMap const& Map() const {
            return value.Map();
        }

        template <class Type>
        SP<Type> Find(String type, String id) const {
            try {
                auto& typeMap = value.Map().at(type);
                auto result = typeMap.at(id);
                return DCAST<Type>(result.resource);
            } catch (...) {
                return nullptr;
            }
        }

        /// @return Returns a set of the resources with the specified ids and type
        template <class Type>
        UnorderedSet<SP<Type>> TypeSet(String type, UnorderedSet<String> const& ids) const {
            VectorList<SP<Type>> result = TypeList<Type>(type, ids);
            UnorderedSet<SP<Type>> _result(result.begin(), result.end());
            return _result;
        }

        /// @return Returns a list of the resources with the specified ids and type
        template <class Type>
        VectorList<SP<Type>> TypeList(String type, UnorderedSet<String> const& ids) const {
            VectorList<String> _ids(ids.begin(), ids.end());
            VectorList<SP<Type>> result =
                CompactMap<SP<Type>>(_ids, [=, this](auto& id) { return Find<Type>(type, id); });
            return result;
        }

        /// @return Returns the first resources of the correct type
        template <class Type>
        SP<Type> TypeFirst(String type) const {
            try {
                auto& typeMap = value.Map().at(type);
                GUARDR(typeMap.size() > 0, {})

                return DCAST<Type>(typeMap.begin()->second.resource);
            } catch (...) {}

            return {};
        }

        SP<SomeTexture> FindTexture(String id) const {
            return Find<SomeTexture>(ResourceType::Texture, id);
        }

        SP<SomeShaderProgram> FindShaderProgram(String id) const {
            return Find<SomeShaderProgram>(ResourceType::ShaderProgram, id);
        }
    };

    /// @return Returns a font that matches the specification
    SP<Font> FindFont(ResourceCatalog const& resources, FontSpec spec);

    /// @return Returns the first font that matches this font size
    SP<Font> FindFontWithSize(ResourceCatalog const& resources, float size);

    /// @return Returns a font with the specified id
    SP<Font> FindFontWithResourceId(ResourceCatalog const& resources, String id);
} // namespace PJ
