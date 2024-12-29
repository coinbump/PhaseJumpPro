#pragma once

#include "Bitmap.h"
#include "SomeOperation.h"
#include "TypeClass.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Bitmap operations operate exclusively on RGBABitmap.
    /// This allows us to write operations that don't have to worry about supporting all possible
    /// pixel formats
    using BitmapOperation = ResultOperation<RGBABitmap&, Void>;

    /// Creates common bitmap operations
    namespace BitmapOperations {
        using BitmapType = RGBABitmap;
        using PixelFormat = PJ::PixelFormat::RGBA8888;
        using Pixel = PixelFormat::Pixel;

        UP<BitmapOperation> FlipV();

    } // namespace BitmapOperations

    /// Used to register a type of operation that can be instantiated as needed
    class BitmapOperationClass : public TypeClass<BitmapOperation> {
    public:
        using Base = TypeClass<BitmapOperation>;

        BitmapOperationClass(String id, String name, FactoryFunc factoryFunc) :
            Base(id, factoryFunc) {
            core.name = name;
        }
    };
} // namespace PJ
