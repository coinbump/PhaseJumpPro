#pragma once

#include "Color.h"
#include "Data.h"
#include "Rect.h"
#include "Vector2.h"
#include "VectorList.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <span>
#include <type_traits>

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    enum class ColorComponent { Red, Green, Blue, Alpha };

    enum class CopyType { Copy, None };

    enum class BitmapOrientation {
        /// Default bitmap orientation
        Standard,

        /// Bitmap is flipped vertically (used for graphics pipelines like OpenGL)
        Flip
    };

    BitmapOrientation Flipped(BitmapOrientation value);

    namespace PixelFormat {
        struct RGBA8888 {
            using Pixel = RGBAColor;

            static constexpr int ColorComponentBitSize(ColorComponent) {
                return 8;
            }

            static constexpr int PixelSize() {
                return 4;
            }

            static constexpr bool HasAlpha() {
                return true;
            }

            static Pixel FromColor(Color color) {
                return RGBAColor(color);
            }
        };

        struct BGRA8888 {
            using Pixel = BGRAColor;

            static constexpr int ColorComponentBitSize(ColorComponent) {
                return 8;
            }

            static constexpr int PixelSize() {
                return 4;
            }

            static constexpr bool HasAlpha() {
                return true;
            }

            static Pixel FromColor(Color color) {
                return BGRAColor(color.r, color.g, color.b, color.a);
            }
        };
    }; // namespace PixelFormat

    /// Interface to bitmap object
    class SomeBitmap : public Base {
    public:
        using This = SomeBitmap;

        virtual ~SomeBitmap() {};

        virtual void* DataPtr() = 0;
        virtual void const* DataPtr() const = 0;
        virtual Vector2Int Size() const = 0;
        virtual size_t DataSize() const = 0;
        virtual int PixelSize() const = 0;
        virtual bool HasAlpha() const = 0;
    };

    /// Stores bitmap images in memory as pixels
    template <class PixelFormat = PixelFormat::RGBA8888>
    class Bitmap final : public SomeBitmap {
    public:
        using Base = SomeBitmap;
        using This = Bitmap;
        using Pixel = typename PixelFormat::Pixel;

        static_assert(
            std::is_trivially_copyable_v<Pixel>,
            "Pixel must be trivially copyable (Bitmap uses memcpy / std::copy_n)"
        );

        // Don't allow copies (pixel buffers can be large); moves are fine.
        DELETE_COPY(Bitmap)
        DEFAULT_MOVE(Bitmap)

    protected:
        VectorList<Pixel> pixels;
        Vector2Int size;

    public:
        BitmapOrientation orientation{};

        struct Config {
            BitmapOrientation orientation{};
            Vector2Int size;

            /// Optional source pixels copied into the bitmap at construction.
            /// Interpreted as raw bytes; `pixelsDataSize` is the size of the buffer in bytes.
            void const* pixels{};
            size_t pixelsDataSize{};
        };

        Bitmap() {}

        Bitmap(Config const& config) :
            orientation(config.orientation) {
            size = config.size;
            GUARD(size.x > 0 && size.y > 0)

            // Avoid overflow for large bitmaps
            size_t pixelCount = (size_t)size.x * (size_t)size.y;
            pixels.resize(pixelCount);

            if (config.pixels && config.pixelsDataSize > 0) {
                auto totalSize = (size_t)PixelFormat::PixelSize() * pixels.size();
                memcpy(pixels.data(), config.pixels, std::min(totalSize, config.pixelsDataSize));
            }
        }

        template <class Check>
        bool AnyPixelsOnScanLine(int y, Check check) {
            auto lineData = ScanLineData(y);
            GUARDR(!lineData.empty(), false)

            for (auto& pixel : lineData) {
                if (check(pixel)) {
                    return true;
                }
            }

            return false;
        }

        template <class Check>
        std::optional<int> FirstPixelIndexOnScanLine(int y, Check check) {
            auto lineData = ScanLineData(y);
            GUARDR(!lineData.empty(), std::nullopt)

            for (size_t i = 0; i < lineData.size(); i++) {
                if (check(lineData[i])) {
                    return i;
                }
            }

            return std::nullopt;
        }

        std::span<Pixel> Data() {
            return pixels.size() > 0 ? std::span<Pixel>(pixels) : std::span<Pixel>();
        }

        std::span<Pixel const> Data() const {
            return pixels.size() > 0 ? std::span<Pixel const>(pixels) : std::span<Pixel const>();
        }

        std::span<Pixel> ScanLineData(int y) {
            GUARDR(IsValidY(y), std::span<Pixel>());

            return std::span<Pixel>(pixels.data() + (int64_t)size.x * (int64_t)y, size.x);
        }

        std::span<Pixel const> ScanLineData(int y) const {
            GUARDR(IsValidY(y), std::span<Pixel const>());

            return std::span<Pixel const>(pixels.data() + (int64_t)size.x * (int64_t)y, size.x);
        }

        bool IsValidY(int y) const {
            return y >= 0 && y < size.y;
        }

        VectorList<Pixel> const& Pixels() const {
            return pixels;
        }

        VectorList<Pixel>& Pixels() {
            return pixels;
        }

        int Width() const {
            return size.x;
        }

        int Height() const {
            return size.y;
        }

        size_t ColorComponentBitSize(ColorComponent component) const {
            return PixelFormat::ColorComponentBitSize(component);
        }

        std::span<Pixel> PixelDataAt(Vector2Int loc, size_t count) {
            auto index = LocToIndex(loc);
            GUARDR(index, std::span<Pixel>())

            auto maxCount = pixels.size() - (size_t)*index;
            auto minCount = std::min(count, maxCount);
            return std::span<Pixel>(pixels.data() + *index, minCount);
        }

        std::span<Pixel const> PixelDataAt(Vector2Int loc, size_t count) const {
            auto index = LocToIndex(loc);
            GUARDR(index, std::span<Pixel const>())

            auto maxCount = pixels.size() - (size_t)*index;
            auto minCount = std::min(count, maxCount);
            return std::span<Pixel const>(pixels.data() + *index, minCount);
        }

        std::optional<int64_t> LocToIndex(Vector2Int loc) const {
            GUARDR(loc.x >= 0 && loc.y >= 0, std::nullopt)
            GUARDR(loc.x < size.x && loc.y < size.y, std::nullopt)

            int64_t result = (int64_t)size.x * (int64_t)loc.y + (int64_t)loc.x;
            GUARDR(result < (int64_t)pixels.size(), std::nullopt)

            return result;
        }

        Color PixelColorAt(Vector2Int loc) const {
            Color result = Color::clear;

            auto pixelData = PixelDataAt(loc, 1);
            GUARDR(!pixelData.empty(), result)

            return pixelData[0];
        }

        RGBAColor PixelColor32At(Vector2Int loc) const {
            RGBAColor result(0, 0, 0, 0);

            auto pixelData = PixelDataAt(loc, 1);
            GUARDR(!pixelData.empty(), result);

            return pixelData[0];
        }

        void SetPixelColor(Vector2Int loc, Color color) {
            auto pixelData = PixelDataAt(loc, 1);
            GUARD(!pixelData.empty());

            pixelData[0] = PixelFormat::FromColor(color);
        }

        /// Sets every pixel in the bitmap to `color`.
        void Fill(Color color) {
            Pixel pixel = PixelFormat::FromColor(color);
            std::fill(pixels.begin(), pixels.end(), pixel);
        }

        void Resize(Vector2Int size, CopyType copyType = CopyType::Copy) {
            GUARD(this->size != size)
            GUARD(size.x > 0 && size.y > 0)

            auto oldSize = this->size;

            // Fast path: no need to preserve old contents.
            if (copyType == CopyType::None || pixels.empty()) {
                pixels = VectorList<Pixel>((size_t)size.x * (size_t)size.y);
                this->size = size;
                return;
            }

            VectorList<Pixel> oldPixels = std::move(pixels);
            pixels = VectorList<Pixel>((size_t)size.x * (size_t)size.y);
            this->size = size;

            auto copyWidth = std::min(oldSize.x, size.x);
            auto copyHeight = std::min(oldSize.y, size.y);
            GUARD(copyWidth > 0 && copyHeight > 0)

            for (int y = 0; y < copyHeight; y++) {
                Pixel const* srcLine = oldPixels.data() + (size_t)oldSize.x * (size_t)y;
                auto destLine = PixelDataAt(Vector2Int(0, y), (size_t)copyWidth);
                std::copy_n(srcLine, copyWidth, destLine.begin());
            }
        }

        Pixel* operator[](size_t index) {
            GUARDR(index < this->pixels.size(), nullptr)
            return &pixels[index];
        }

        Pixel const* operator[](size_t index) const {
            GUARDR(index < this->pixels.size(), nullptr)
            return &pixels[index];
        }

        Pixel* operator[](Vector2Int loc) {
            auto index = LocToIndex(loc);
            GUARDR(index, nullptr)
            return &(pixels[*index]);
        }

        Pixel const* operator[](Vector2Int loc) const {
            auto index = LocToIndex(loc);
            GUARDR(index, nullptr)
            return &(pixels[*index]);
        }

        // MARK: SomeBitmap

        void* DataPtr() override {
            return pixels.data();
        }

        void const* DataPtr() const override {
            return pixels.data();
        }

        size_t DataSize() const override {
            return (size_t)PixelFormat::PixelSize() * pixels.size();
        }

        int PixelSize() const override {
            return PixelFormat::PixelSize();
        }

        Vector2Int Size() const override {
            return size;
        }

        bool HasAlpha() const override {
            return PixelFormat::HasAlpha();
        }
    };

    using RGBABitmap = Bitmap<PixelFormat::RGBA8888>;
    using BGRABitmap = Bitmap<PixelFormat::BGRA8888>;

} // namespace PJ
