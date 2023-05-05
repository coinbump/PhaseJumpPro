#include "Bitmap.h"

using namespace std;
using namespace PJ;

//void Bitmap::Erase(RectInt rect)
//{
//    uint32_t pixelSize = PixelSize();
//    uint32_t lineSize = pixelSize * rect.Width();
//
//    int x = rect.origin.x;
//
//    for (int y = rect.origin.y; y < min(rect.Bottom() + 1, size.y); y++) {
//        auto pixelData = PixelDataAt(x, y);
//        memset(pixelData, 0, lineSize);
//    }
//}
