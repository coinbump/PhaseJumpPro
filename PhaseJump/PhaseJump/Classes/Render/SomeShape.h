#pragma once

#include "CapsuleMeshBuilder.h"
#include "EllipseMeshBuilder.h"
#include "QuadMeshBuilder.h"
#include "SomeMeshBuilder.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/23/26
 */
namespace PJ {
    /// Describes a 2D shape that can produce its mesh builder on demand.
    class SomeShape {
    public:
        using This = SomeShape;

        Vector2 worldSize;

        SomeShape() {}

        SomeShape(Vector2 worldSize) :
            worldSize(worldSize) {}

        virtual ~SomeShape() = default;

        /// @return Returns a mesh builder that produces the shape's mesh
        virtual UP<SomeMeshBuilder> SomeMeshBuilder() = 0;

        /// @return Returns a deep copy of this shape
        virtual UP<SomeShape> Clone() const = 0;
    };

    /// Ellipse shape. Produces an EllipseMeshBuilder
    class EllipseShape : public SomeShape {
    public:
        using Base = SomeShape;
        using This = EllipseShape;

        EllipseShape() {}

        EllipseShape(Vector2 worldSize) :
            Base(worldSize) {}

        // MARK: SomeShape

        UP<PJ::SomeMeshBuilder> SomeMeshBuilder() override {
            return NEW<EllipseMeshBuilder>(worldSize);
        }

        UP<PJ::SomeShape> Clone() const override {
            return NEW<EllipseShape>(*this);
        }
    };

    /// Rectangle shape. Produces a QuadMeshBuilder
    class RectShape : public SomeShape {
    public:
        using Base = SomeShape;
        using This = RectShape;

        RectShape() {}

        RectShape(Vector2 worldSize) :
            Base(worldSize) {}

        // MARK: SomeShape

        UP<PJ::SomeMeshBuilder> SomeMeshBuilder() override {
            return NEW<QuadMeshBuilder>(worldSize);
        }

        UP<PJ::SomeShape> Clone() const override {
            return NEW<RectShape>(*this);
        }
    };

    /// UI capsule shape. Always oriented horizontally, because capsules in UI are generally
    /// horizontal.
    class UICapsuleShape : public SomeShape {
    public:
        using Base = SomeShape;
        using This = UICapsuleShape;

        UICapsuleShape() {}

        UICapsuleShape(Vector2 worldSize) :
            Base(worldSize) {}

        // MARK: SomeShape

        UP<PJ::SomeMeshBuilder> SomeMeshBuilder() override {
            return NEW<CapsuleMeshBuilder>(CapsuleMeshBuilder::Config{ .axis = Axis2D::X,
                                                                       .worldSize = worldSize });
        }

        UP<PJ::SomeShape> Clone() const override {
            return NEW<UICapsuleShape>(*this);
        }
    };
} // namespace PJ
