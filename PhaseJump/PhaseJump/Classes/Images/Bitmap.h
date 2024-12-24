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

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    enum class ColorComponent { Red, Green, Blue, Alpha };

    namespace PixelFormat {
        struct RGBA8888 {
            using Pixel = RGBAColor;

            int ColorComponentBitSize(ColorComponent component) const {
                return 8;
            }

            int PixelSize() const {
                return 4;
            }
        };

        struct BGRA8888 {
            using Pixel = BGRAColor;

            int ColorComponentBitSize(ColorComponent component) const {
                return 8;
            }

            int PixelSize() const {
                return 4;
            }
        };
    }; // namespace PixelFormat

    /// Interface to bitmap object
    class SomeBitmap {
    public:
        virtual ~SomeBitmap() {};

        virtual void* DataPtr() const = 0;
        virtual Vector2Int const& Size() const = 0;
        virtual size_t DataSize() const = 0;
        virtual int PixelSize() const = 0;
    };

    /// Stores bitmap images in memory as pixels
    template <class PixelFormat = PixelFormat::RGBA8888>
    class Bitmap : public SomeBitmap {
    public:
        using Pixel = typename PixelFormat::Pixel;

        // Don't allow copies
        DELETE_COPY(Bitmap)

    protected:
        VectorList<Pixel> pixels;
        PixelFormat pixelFormat;
        Vector2Int size;

    public:
        Bitmap() {}

        Bitmap(Vector2Int size, void* pixels = nullptr, size_t pixelsDataSize = 0) {
            GUARD(size.x > 0 && size.y > 0)

            this->size = size;
            auto pixelCount = size.x * size.y;
            this->pixels.resize(pixelCount);

            GUARD(pixels && pixelsDataSize > 0)
            auto totalSize = pixelFormat.PixelSize() * this->pixels.size();
            memcpy(this->pixels.data(), pixels, std::min(totalSize, pixelsDataSize));
        }

        Bitmap(Vector2Int size, PJ::Data<uint32_t> const& data) :
            Bitmap(size, data.Pointer(), data.Size()) {}

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
            return pixels.size() > 0 ? std::span<Pixel const>(pixels) : std::span<Pixel>();
        }

        std::span<Pixel> ScanLineData(int y) {
            GUARDR(IsValidY(y), std::span<Pixel>());

            return std::span<Pixel>(pixels.data() + (size.x * y), size.x);
        }

        std::span<Pixel const> ScanLineData(int y) const {
            GUARDR(IsValidY(y), std::span<Pixel>());

            return std::span<Pixel const>(pixels.data() + (size.x * y), size.x);
        }

        bool IsValidY(int y) const {
            GUARDR(pixels.size() > 0 && size.x > 0 && size.y > 0, false);
            GUARDR(y >= 0 && y < size.y, false);

            return true;
        }

        VectorList<Pixel> const& Pixels() const {
            return pixels;
        }

        VectorList<Pixel>& Pixels() {
            return pixels;
        }

        size_t ColorComponentBitSize(ColorComponent component) const {
            return pixelFormat.ColorComponentBitSize(component);
        }

        bool HasAlpha() const {
            return true;
        }

        std::span<Pixel> PixelDataAt(Vector2Int loc, size_t count) const {
            auto index = LocToIndex(loc);
            GUARDR(index, std::span<Pixel>())

            // Check: pixels.size is 2, index is 1, max # of items copied is 1 (2 - 1)
            auto maxCount = pixels.size() - *index;
            auto minCount = std::min(count, maxCount);
            return std::span<Pixel>((Pixel*)pixels.data() + *index, minCount);
        }

        std::optional<int> LocToIndex(Vector2Int loc) const {
            GUARDR(loc.x >= 0 && loc.y >= 0, std::nullopt)
            GUARDR(loc.x < size.x && loc.y < size.y, std::nullopt)

            auto result = size.x * loc.y + loc.x;
            GUARDR(result < pixels.size(), std::nullopt)

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

            RGBAColor color32 = color;
            pixelData[0] = color32;
        }

        void Resize(Vector2Int size) {
            GUARD(this->size != size)
            GUARD(size.x > 0 && size.y > 0)

            auto oldSize = this->size;
            auto oldPixels = pixels;

            size_t vectorSize = size.x * size.y;
            pixels = VectorList<Pixel>(vectorSize);

            this->size = size;

            if (oldPixels.size() == 0) {
                return;
            }

            // FUTURE: update with std::span and std::copy
            char* sourceData = (char*)oldPixels.data();
            size_t sourceLineSize = PixelSize() * oldSize.x;
            size_t destLineSize = PixelSize() * size.x;

            auto lineCopySize = std::min(sourceLineSize, destLineSize);
            GUARD(lineCopySize > 0)

            auto copyHeight = std::min(oldSize.y, size.y);
            GUARD(copyHeight > 0)

            for (int y = 0; y < copyHeight; y++) {
                memcpy(
                    PixelDataAt(Vector2Int(0, y), lineCopySize).data(), sourceData, lineCopySize
                );
                sourceData += sourceLineSize;
            }
        }

        Pixel* operator[](size_t index) {
            GUARDR(index >= 0 && index < this->pixels.size(), nullptr)
            return &pixels[index];
        }

        Pixel const* operator[](size_t index) const {
            GUARDR(index >= 0 && index < this->pixels.size(), nullptr)
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

        void* DataPtr() const override {
            return (void*)pixels.data();
        }

        size_t DataSize() const override {
            return pixelFormat.PixelSize() * pixels.size();
        }

        int PixelSize() const override {
            return pixelFormat.PixelSize();
        }

        Vector2Int const& Size() const override {
            return size;
        }
    };

    using RGBABitmap = Bitmap<PixelFormat::RGBA8888>;
    using BGRABitmap = Bitmap<PixelFormat::BGRA8888>;

} // namespace PJ
