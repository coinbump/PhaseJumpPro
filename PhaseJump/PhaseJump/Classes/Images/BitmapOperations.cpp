#include "BitmapOperations.h"

using namespace std;
using namespace PJ;

UP<BitmapOperation> BitmapOperations::FlipV(BitmapType& bitmap) {
    return NEW<BitmapOperation>(&bitmap, [](auto& operation) {
        GUARD(operation.core)
        BitmapType& bitmap = *operation.core;

        GUARD(bitmap.Size().x > 0 && bitmap.Size().y > 0)

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
    });
}
