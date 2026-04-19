#include "BitmapOperations.h"

using namespace std;
using namespace PJ;

namespace {
    using Pixel = BitmapOperations::Pixel;

    /// ITU-R BT.709 luminance for use as the dither input scalar.
    float Luminance(Pixel const& pixel) {
        return 0.2126f * pixel.redFloat() + 0.7152f * pixel.greenFloat() +
               0.0722f * pixel.blueFloat();
    }

    /// 1-bit black/white palette quantizer. Returns 0 (black) or 1 (white).
    float FindClosestPaletteColor1Bit(float gray) {
        return gray < 0.5f ? 0.0f : 1.0f;
    }

    void DistributeError(
        VectorList<float>& buffer, int width, int height, int x, int y, float error, float weight
    ) {
        GUARD(x >= 0 && x < width && y >= 0 && y < height)
        buffer[(size_t)y * width + x] += error * weight;
    }
} // namespace

UP<BitmapOperation> BitmapOperations::NewFlipVertical(UpdateOrientationType updateOrientationType) {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [=](auto& operation, auto& bitmap) {
            GUARDR(bitmap.Size().x > 0 && bitmap.Size().y > 0, Void{})

            for (int i = 0; i < bitmap.Size().y; i++) {
                auto j = bitmap.Size().y - i - 1;
                GUARD_BREAK(j > i)

                std::span<Pixel> iData = bitmap.ScanLineData(i);
                std::span<Pixel> jData = bitmap.ScanLineData(j);

                VectorList<Pixel> swapLine;
                swapLine.assign(iData.begin(), iData.end());

                std::copy(jData.begin(), jData.end(), iData.begin());
                std::copy(swapLine.begin(), swapLine.end(), jData.begin());
            }

            // Keep track of bitmap orientation for graphic pipelines
            if (updateOrientationType == UpdateOrientationType::Update) {
                bitmap.orientation = Flipped(bitmap.orientation);
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewFlipHorizontal() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [=](auto& operation, auto& bitmap) {
            GUARDR(bitmap.Size().x > 0 && bitmap.Size().y > 0, Void{})

            for (int y = 0; y < bitmap.Size().y; y++) {
                std::span<Pixel> lineData = bitmap.ScanLineData(y);
                std::reverse(lineData.begin(), lineData.end());
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewRotateRight() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [=](auto& operation, auto& bitmap) {
            int width = bitmap.Size().x;
            int height = bitmap.Size().y;
            GUARDR(width > 0 && height > 0, Void{})

            auto srcData = bitmap.Data();
            VectorList<Pixel> src(srcData.begin(), srcData.end());

            bitmap.Resize(Vector2Int(height, width), CopyType::None);

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto srcIndex = y * width + x;
                    auto dstLoc = Vector2Int(height - 1 - y, x);
                    auto dstData = bitmap.PixelDataAt(dstLoc, 1);
                    dstData[0] = src[srcIndex];
                }
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewRotateLeft() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [=](auto& operation, auto& bitmap) {
            int width = bitmap.Size().x;
            int height = bitmap.Size().y;
            GUARDR(width > 0 && height > 0, Void{})

            auto srcData = bitmap.Data();
            VectorList<Pixel> src(srcData.begin(), srcData.end());

            bitmap.Resize(Vector2Int(height, width), CopyType::None);

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto srcIndex = y * width + x;
                    auto dstLoc = Vector2Int(y, width - 1 - x);
                    auto dstData = bitmap.PixelDataAt(dstLoc, 1);
                    dstData[0] = src[srcIndex];
                }
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewFloydSteinbergDither() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [](auto& operation, auto& bitmap) {
            int width = bitmap.Size().x;
            int height = bitmap.Size().y;
            GUARDR(width > 0 && height > 0, Void{})

            auto data = bitmap.Data();

            VectorList<float> gray((size_t)width * height);
            VectorList<int> alpha((size_t)width * height);
            for (int i = 0; i < width * height; i++) {
                gray[i] = Luminance(data[i]);
                alpha[i] = data[i].a();
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    size_t index = (size_t)y * width + x;
                    float oldValue = gray[index];
                    float newValue = FindClosestPaletteColor1Bit(oldValue);
                    gray[index] = newValue;
                    float error = oldValue - newValue;

                    DistributeError(gray, width, height, x + 1, y, error, 7.0f / 16.0f);
                    DistributeError(gray, width, height, x - 1, y + 1, error, 3.0f / 16.0f);
                    DistributeError(gray, width, height, x, y + 1, error, 5.0f / 16.0f);
                    DistributeError(gray, width, height, x + 1, y + 1, error, 1.0f / 16.0f);
                }
            }

            for (int i = 0; i < width * height; i++) {
                int component = gray[i] >= 0.5f ? 255 : 0;
                data[i] = RGBAColor(component, component, component, alpha[i]);
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewAtkinsonDither() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [](auto& operation, auto& bitmap) {
            int width = bitmap.Size().x;
            int height = bitmap.Size().y;
            GUARDR(width > 0 && height > 0, Void{})

            auto data = bitmap.Data();

            VectorList<float> gray((size_t)width * height);
            VectorList<int> alpha((size_t)width * height);
            for (int i = 0; i < width * height; i++) {
                gray[i] = Luminance(data[i]);
                alpha[i] = data[i].a();
            }

            constexpr float weight = 1.0f / 8.0f;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    size_t index = (size_t)y * width + x;
                    float oldValue = gray[index];
                    float newValue = FindClosestPaletteColor1Bit(oldValue);
                    gray[index] = newValue;
                    float error = oldValue - newValue;

                    // Atkinson 1-bit kernel (6/8 distributed, 2/8 discarded):
                    //         X * *
                    //       * * *
                    //         *
                    DistributeError(gray, width, height, x + 1, y, error, weight);
                    DistributeError(gray, width, height, x + 2, y, error, weight);
                    DistributeError(gray, width, height, x - 1, y + 1, error, weight);
                    DistributeError(gray, width, height, x, y + 1, error, weight);
                    DistributeError(gray, width, height, x + 1, y + 1, error, weight);
                    DistributeError(gray, width, height, x, y + 2, error, weight);
                }
            }

            for (int i = 0; i < width * height; i++) {
                int component = gray[i] >= 0.5f ? 255 : 0;
                data[i] = RGBAColor(component, component, component, alpha[i]);
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewBayerDither() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [](auto& operation, auto& bitmap) {
            int width = bitmap.Size().x;
            int height = bitmap.Size().y;
            GUARDR(width > 0 && height > 0, Void{})

            // Standard 8x8 Bayer ordered-dither matrix, values in [0, 63].
            constexpr int bayer8x8[8][8] = {
                { 0, 32, 8, 40, 2, 34, 10, 42 },  { 48, 16, 56, 24, 50, 18, 58, 26 },
                { 12, 44, 4, 36, 14, 46, 6, 38 }, { 60, 28, 52, 20, 62, 30, 54, 22 },
                { 3, 35, 11, 43, 1, 33, 9, 41 },  { 51, 19, 59, 27, 49, 17, 57, 25 },
                { 15, 47, 7, 39, 13, 45, 5, 37 }, { 63, 31, 55, 23, 61, 29, 53, 21 }
            };

            auto data = bitmap.Data();

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    size_t index = (size_t)y * width + x;
                    float luminance = Luminance(data[index]);

                    // Per-pixel threshold in [~0.008, ~0.992]. Re-center as a bias around
                    // 0.5 so we can reuse FindClosestPaletteColor1Bit's 0.5 cutoff.
                    float threshold = (bayer8x8[y % 8][x % 8] + 0.5f) / 64.0f;
                    float biased = luminance - threshold + 0.5f;
                    int component = FindClosestPaletteColor1Bit(biased) >= 0.5f ? 255 : 0;

                    int alpha = data[index].a();
                    data[index] = RGBAColor(component, component, component, alpha);
                }
            }

            return Void{};
        } });
}

UP<BitmapOperation> BitmapOperations::NewStamp(BitmapType const& stampBitmap, Vector2Int position) {
    // Snapshot the stamp's pixels now so the operation doesn't depend on the source
    // bitmap still being alive when it runs.
    Vector2Int stampSize = stampBitmap.Size();
    auto stampData = stampBitmap.Data();
    VectorList<Pixel> stampPixels(stampData.begin(), stampData.end());

    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [stampSize, stampPixels = std::move(stampPixels),
                    position](auto& operation, auto& bitmap) {
            Vector2Int destSize = bitmap.Size();
            GUARDR(destSize.x > 0 && destSize.y > 0, Void{})
            GUARDR(stampSize.x > 0 && stampSize.y > 0, Void{})

            int destStartX = std::max(position.x, 0);
            int destStartY = std::max(position.y, 0);
            int destEndX = std::min(position.x + stampSize.x, destSize.x);
            int destEndY = std::min(position.y + stampSize.y, destSize.y);

            GUARDR(destEndX > destStartX && destEndY > destStartY, Void{})

            int copyWidth = destEndX - destStartX;
            int copyHeight = destEndY - destStartY;

            int srcStartX = destStartX - position.x;
            int srcStartY = destStartY - position.y;

            for (int y = 0; y < copyHeight; y++) {
                auto destLine =
                    bitmap.PixelDataAt(Vector2Int(destStartX, destStartY + y), (size_t)copyWidth);
                Pixel const* srcBase =
                    stampPixels.data() + (srcStartY + y) * stampSize.x + srcStartX;

                std::copy(srcBase, srcBase + copyWidth, destLine.begin());
            }

            return Void{};
        } });
}
