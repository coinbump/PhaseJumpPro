#pragma once

#include "Color.h"
#include "Data.h"
#include "Rect.h"
#include "Vector2.h"
#include "VectorList.h"
#include <algorithm>
#include <memory>
#include <optional>

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

            size_t ColorComponentBitSize(ColorComponent component) const {
                return 8;
            }

            size_t PixelSize() const {
                return 4;
            }
        };

        struct BGRA8888 {
            using Pixel = BGRAColor;

            size_t ColorComponentBitSize(ColorComponent component) const {
                return 8;
            }

            size_t PixelSize() const {
                return 4;
            }
        };
    }; // namespace PixelFormat

    template <class PixelFormat = PixelFormat::RGBA8888>
    class Bitmap {
    public:
        using Pixel = typename PixelFormat::Pixel;

    protected:
        VectorList<Pixel> pixels;
        PixelFormat pixelFormat;
        Vector2Int size;

    public:
        Pixel* Data() const {
            return pixels.size() > 0 ? const_cast<Pixel*>(pixels.data()) : nullptr;
        }

        size_t DataSize() const {
            return pixels.size() * PixelSize();
        }

        Vector2Int const& Size() const {
            return size;
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

        size_t PixelSize() const {
            return pixelFormat.PixelSize();
        }

        bool HasAlpha() const {
            return true;
        }

        Bitmap() {}

        Bitmap(Vector2Int size, void* pixels = nullptr, size_t pixelsDataSize = 0) {
            this->size = size;
            if (size.x <= 0 || size.y <= 0) {
                return;
            }

            auto pixelCount = size.x * size.y;
            this->pixels.resize(pixelCount);

            GUARD(pixels && pixelsDataSize > 0)

            auto totalSize = pixelFormat.PixelSize() * this->pixels.size();

            memcpy(this->pixels.data(), pixels, std::min(totalSize, pixelsDataSize));
        }

        Pixel* PixelDataAt(Vector2Int loc) const {
            auto index = LocToIndex(loc);
            GUARDR(index, nullptr)

            return (Pixel*)pixels.data() + index.value();
        }

        std::optional<uint32_t> LocToIndex(Vector2Int loc) const {
            GUARDR(loc.x >= 0 && loc.y >= 0, std::nullopt)
            GUARDR(loc.x < size.x && loc.y < size.y, std::nullopt)

            auto result = size.x * loc.y + loc.x;
            GUARDR(result < pixels.size(), std::nullopt)

            return result;
        }

        Color PixelColorAt(Vector2Int loc) const {
            Color result = Color::clear;

            auto pixelData = PixelDataAt(loc);
            GUARDR(pixelData, result)

            return *pixelData;
        }

        RGBAColor PixelColor32At(Vector2Int loc) const {
            RGBAColor result(0, 0, 0, 0);

            auto pixelData = PixelDataAt(loc);
            GUARDR(pixelData, result);

            return *pixelData;
        }

        void SetPixelColor(Vector2Int loc, Color color) {
            auto pixelData = PixelDataAt(loc);
            GUARD(pixelData);

            RGBAColor color32 = color;
            *pixelData = color32;
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

            char* sourceData = (char*)oldPixels.data();
            size_t sourceLineSize = PixelSize() * oldSize.x;
            size_t destLineSize = PixelSize() * size.x;

            auto lineCopySize = std::min(sourceLineSize, destLineSize);
            GUARD(lineCopySize > 0)

            auto copyHeight = std::min(oldSize.y, size.y);
            GUARD(copyHeight > 0)

            for (int y = 0; y < copyHeight; y++) {
                memcpy(PixelDataAt(Vector2Int(0, y)), sourceData, lineCopySize);
                sourceData += sourceLineSize;
            }
        }

        Pixel* operator[](size_t index) {
            GUARDR(index >= 0 && index < this->pixels.size(), nullptr)
            return &pixels[index];
        }

        Pixel* operator[](Vector2Int loc) {
            auto index = LocToIndex(loc);
            GUARDR(index, nullptr)
            return (*this)[index.value()];
        }
    };

    using RGBABitmap = Bitmap<PixelFormat::RGBA8888>;
    using BGRABitmap = Bitmap<PixelFormat::BGRA8888>;
} // namespace PJ
