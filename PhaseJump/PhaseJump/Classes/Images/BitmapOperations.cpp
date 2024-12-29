#include "BitmapOperations.h"

using namespace std;
using namespace PJ;

UP<BitmapOperation> BitmapOperations::FlipV() {
    return NEW<BitmapOperation>(BitmapOperation::Config{
        .runFunc = [](auto& operation, auto& bitmap) {
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
            bitmap.orientation = Flipped(bitmap.orientation);

            return Void{};
        } });
}
