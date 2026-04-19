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
    enum class UpdateOrientationType { Update, None };

    /// Bitmap operations operate exclusively on RGBABitmap.
    /// This allows us to write operations that don't have to worry about supporting all possible
    /// pixel formats
    using BitmapOperation = ResultOperation<RGBABitmap&, Void>;

    /// Creates common bitmap operations
    namespace BitmapOperations {
        using BitmapType = RGBABitmap;
        using PixelFormat = PJ::PixelFormat::RGBA8888;
        using Pixel = PixelFormat::Pixel;

        UP<BitmapOperation> NewFlipVertical(UpdateOrientationType updateOrientationType);
        UP<BitmapOperation> NewFlipHorizontal();
        UP<BitmapOperation> NewRotateRight();
        UP<BitmapOperation> NewRotateLeft();

        /// Reduces the bitmap to a 1-bit black/white palette using Floyd-Steinberg dither.
        /// Alpha is preserved.
        UP<BitmapOperation> NewFloydSteinbergDither();

        /// Reduces the bitmap to a 1-bit black/white palette using Atkinson dithering.
        /// Alpha is preserved.
        UP<BitmapOperation> NewAtkinsonDither();

        /// Reduces the bitmap to a 1-bit black/white palette using an 8x8 ordered
        /// Bayer matrix threshold (recursive subdivision pattern). Unlike error-diffusion
        /// dithers, every pixel is decided independently from its (x, y) position, which
        /// produces the characteristic stable, repeating cross-hatch look. Alpha is preserved.
        UP<BitmapOperation> NewBayerDither();

        /// Copies pixels from `stampBitmap` into the operation's bitmap with `stampBitmap`'s
        /// origin placed at `position`. Any portion of the stamp that falls outside the
        /// destination bitmap's bounds is clipped.
        UP<BitmapOperation> NewStamp(BitmapType const& stampBitmap, Vector2Int position);

    } // namespace BitmapOperations

    /// Used to register a type of operation that can be instantiated as needed
    class BitmapOperationClass final : public TypeClass<BitmapOperation> {
    public:
        using Base = TypeClass<BitmapOperation>;

        BitmapOperationClass(String id, String name, FactoryFunc factoryFunc) :
            Base(id, factoryFunc) {
            _core.name = name;
        }
    };
} // namespace PJ
