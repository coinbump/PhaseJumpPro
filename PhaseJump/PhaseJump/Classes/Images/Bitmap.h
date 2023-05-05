#ifndef PJBITMAP_H
#define PJBITMAP_H

#include "Color.h"
#include "Vector2.h"
#include "VectorList.h"
#include "Data.h"
#include "Rect.h"
#include <memory>
#include <optional>
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    enum class PixelFormat {
        // Standard format. FUTURE: support more pixel formats if needed
        RGBA = 0
    };

    enum class ColorComponent {
        Red,
        Green,
        Blue,
        Alpha
    };

    template <class Pixel>
    class Bitmap {
    protected:
        VectorList<Pixel> pixels;
        PixelFormat pixelFormat = PixelFormat::RGBA;
        Vector2Int size;
        
    public:
        Pixel* Data() const { return pixels.size() > 0 ? const_cast<Pixel*>(pixels.data()) : nullptr; }
        uint32_t DataSize() const { return (uint32_t)pixels.size() * PixelSize(); }

        Vector2Int const& Size() const { return size; }
        VectorList<Pixel> const& Pixels() const { return pixels; }
        VectorList<Pixel>& Pixels() { return pixels; }

        uint32_t ColorComponentSize(ColorComponent component) const { return 1; }
        uint32_t PixelSize() const { return 4; }
        bool HasAlpha() const { return true; }

        // FUTURE: Enable if we add RGBA5551 support
        bool IsAlphaBinary() const { return false; }

        Bitmap() {
        }
        
        Bitmap(Vector2Int size, void* pixels = nullptr) {
            this->size = size;
            if (size.x <=0 || size.y <= 0) { return; }

            auto pixelCount = size.x * size.y;
            this->pixels.resize(pixelCount);

            if (nullptr == pixels) { return; }
            memcpy(this->pixels.data(), pixels, pixelCount * sizeof(Pixel));
        }

        Pixel* PixelDataAt(Vector2Int loc) const
        {
            auto index = LocToIndex(loc);
            if (!index) { return nullptr; }

            return (Pixel*)pixels.data() + index.value();
        }

        std::optional<uint32_t> LocToIndex(Vector2Int loc) const {
            if (loc.x < 0 || loc.y < 0) { return std::nullopt; }
            if (loc.x >= size.x || loc.y >= size.y) { return std::nullopt; }

            auto result = size.x * loc.y + loc.x;
            if (result >= pixels.size()) { return std::nullopt; }

            return result;
        }

        Color PixelColorAt(Vector2Int loc) const
        {
            Color result = Color::clear;

            auto pixelData = PixelDataAt(loc);
            if (nullptr == pixelData) { return result; }

            return *pixelData;
        }

        Color32 PixelColor32At(Vector2Int loc) const
        {
            Color32 result(0, 0, 0, 0);

            auto pixelData = PixelDataAt(loc);
            if (nullptr == pixelData) { return result; }

            return *pixelData;
        }

        void SetPixelColor(Vector2Int loc, Color color)
        {
            auto pixelData = PixelDataAt(loc);
            if (nullptr == pixelData) { return; }

            Color32 color32 = color;
            *pixelData = color32;
        }

        void Resize(Vector2Int size) {
            if (this->size == size) { return; }
            if (size.x <= 0 || size.y <= 0) { return; }

            auto oldSize = this->size;
            auto oldPixels = pixels;

            uint32_t vectorSize = size.x * size.y;
            pixels = VectorList<Pixel>(vectorSize);

            this->size = size;

            if (oldPixels.size() == 0) { return; }

            char* sourceData = (char*)oldPixels.data();
            uint32_t sourceLineSize = PixelSize() * oldSize.x;

            auto copySize = std::min(sourceLineSize, oldSize.x * PixelSize());
            if (0 == copySize) { return; }

            auto copyHeight = std::min(oldSize.y, size.y);
            if (0 == copyHeight) { return; }

            for (int y = 0; y < copyHeight; y++) {
                memcpy(PixelDataAt(Vector2Int(0, y)), sourceData, copySize);
                sourceData += copySize;
            }
        }
    };

    using RGBABitmap = Bitmap<Color32>;
    using BGRABitmap = Bitmap<BGRAColor>;
}

#endif
