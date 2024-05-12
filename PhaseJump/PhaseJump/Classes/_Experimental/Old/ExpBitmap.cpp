//
//  ExpBitmap.cpp
//  PhaseJump
//
//  Created by Jeremy Vineyard on 4/21/23.
//

#if FALSE

#include "ExpBitmap.hpp"

#include "ExpBitmap.h"
#include "Class.h"
#include "ClassNames.h"
#include "StdVector.h"
#include <algorithm>

class FillNode
{
public:
    int x;
    int y;

    FillNode(int x, int y) { this->x = x; this->y = y; }
    bool operator==(FillNode const& node) const { return this->x == node.x && this->y == node.y; }
    bool operator<(FillNode const& node) const {
        if (y == node.y) {
            return x < node.x;
        }
        return y < node.y;
    }

};

void ExpBitmap::ReplaceData(void* data, int width, int height)
{
    Flush();
    data = data;
    mSize = Vector2Int(width, height);

    // NOTE: currently only 32 bit pixels supported
    // FUTURE: support more pixel formats
    dataSize = width*height*PixelSize();
}

Color ExpBitmap::PixelIndexColor(uint32_t i) const {
    uint32_t y = static_cast<uint32_t>(floorf(i/Width()));
    uint32_t x = i - y*Width();

    return ColorAt(x, y);
}

void ExpBitmap::SetPixelIndexColor(uint32_t i, Color color) {
    uint32_t y = static_cast<uint32_t>(floorf(i/Width()));
    uint32_t x = i - y*Width();

    SetPixelColor(x, y, color);
}

ExpBitmap* ExpBitmap::Copy()
{
    ExpBitmap* result = New();
    CopyInto(result);
    return result;
}

void ExpBitmap::CopyInto(ExpBitmap* into) const
{
    into->Flush();
    into->mPixelFormat = mPixelFormat;
    into->mSize = mSize;

    if (dataSize > 0) {
        into->data = malloc(dataSize);
        memcpy(into->data, data, dataSize);
        into->dataSize = dataSize;
    }
}

void ExpBitmap::RenderBitmap(ExpBitmap* bitmap, int x, int y)
{
    if (x < 0 || y < 0 || x >= Width() || y >= Height()) {
        PJLog("ERROR. ExpBitmap.RenderBitmap has bad parameters.");
        return;
    }

    // Temporarily unused. Keep for reference (for future copy mode).
//    int sourceWidth = bitmap->Width();
//    int sourceHeight = bitmap->Height();
//    int maxX = x + sourceWidth - 1;
//    int maxY = y + sourceHeight - 1;
//    int destWidth = Width();
//    int destHeight = Height();
//
//    // Clip the copy size to the destination bounds.
//    maxX = min(maxX, destWidth-1);
//    maxY = min(maxY, destHeight-1);
//
//    int copyWidth = maxX - x + 1;
//    // int copyHeight = maxY - y + 1;
//
//    int bytesPP = PixelSize();
//    int rowSize = bytesPP * copyWidth;

    if (bitmap->mPixelFormat == mPixelFormat) {
        // FUTURE: support faster copy if needed.
//        int sourceY = 0;
//        for (int i = y; i <= maxY; i++, sourceY++) {
//
//            void*    destData = PixelDataAt(x, i);
//            void*    sourceData = bitmap->PixelDataAt(0, sourceY);
//
//            memcpy(destData, sourceData, rowSize);
//        }
        for (int i = 0; i < bitmap->Width(); i++) {
            for (int j = 0; j < bitmap->Height(); j++) {
                int thisX = x+i;
                int thisY = y+j;
                if (thisX >= Width()) {
                    continue;
                }
                if (thisY >= Height()) {
                    continue;
                }

                Color sourceColor = bitmap->ColorAt(i, j);
                if (0 == sourceColor) {
                    continue;
                }
                FColor sourceFColor = ColorToFColor(sourceColor);
                if (0 == sourceFColor.alpha) {
                    continue;
                }
                Color destColor = this->ColorAt(thisX, thisY);
                if (0 == destColor) {
                    SetPixelColor(thisX, thisY, sourceColor);
                    continue;
                }
                FColor destFColor = ColorToFColor(destColor);

                if (1.0 != sourceFColor.alpha) {
                    int breakpoint = 0; breakpoint++;
                }

                if (!bitmap->IsPremultAlpha()) {
                    sourceFColor.PremultAlpha(true);
                }
                if (!IsPremultAlpha()) {
                    destFColor.PremultAlpha(true);
                }

                // http://stackoverflow.com/questions/7438263/alpha-compositing-algorithm-blend-modes
                float alphaFinal = destFColor.alpha + sourceFColor.alpha - destFColor.alpha * sourceFColor.alpha;

                FColor finalColor;
                finalColor.red = sourceFColor.red + destFColor.red * (1.0f-sourceFColor.alpha);
                finalColor.green = sourceFColor.green + destFColor.green * (1.0f-sourceFColor.alpha);
                finalColor.blue = sourceFColor.blue + destFColor.blue * (1.0f-sourceFColor.alpha);
                finalColor.alpha = alphaFinal;

                if (!IsPremultAlpha()) {
                    finalColor.PremultAlpha(false);
                }

                SetPixelColor(thisX, thisY, FColorToColor(finalColor));
            }
        }

    }
    else {
        PJLog("UNIMPLEMENTED. ExpBitmap.RenderBitmap doesn't support non-matching pixel formats.");
    }

}

bool ExpBitmap::AnyPixelsOnHScanLine(int y, bool test, Color testColor) const
{
    bool result = false;

    // FUTURE: optimize if needed.
    for (int i = 0; i < size.x; i++) {
        bool thisTest = ColorAt(i, y) == testColor;
        if (thisTest == test) {
            result = true;
            break;
        }
    }

    return result;
}

bool ExpBitmap::AnyPixelsOnVScanLine(int x, bool test, Color testColor) const
{
    bool result = false;

    // FUTURE: optimize if needed.
    for (int i = 0; i < size.y; i++) {
        bool thisTest = ColorAt(x, i) == testColor;
        if (thisTest == test) {
            result = true;
            break;
        }
    }

    return result;
}

int ExpBitmap::FirstPixelScanlineTToB(int x) const
{
    int result = -1;

    // FUTURE: optimize if needed.
    for (int i = 0; i < size.y; i++) {
        if (ColorAt(x, i) != Color::clear) {
            result = i;
            break;
        }
    }

    return result;
}

int ExpBitmap::FirstPixelScanlineBToT(int x) const
{
    int result = -1;

    // FUTURE: optimize if needed.
    for (int i = size.y-1; i >= 0; i--) {
        Color    c = ColorAt(x, i);
        if (c != Color::clear) {
            result = i;
            break;
        }
    }

    return result;
}

int ExpBitmap::FirstPixelScanlineLToR(int y) const
{
    int result = -1;

    // FUTURE: optimize if needed.
    for (int i = 0; i < size.x; i++) {
        if (ColorAt(i, y) != Color::clear) {
            result = i;
            break;
        }
    }

    return result;
}

int ExpBitmap::FirstPixelScanlineRToL(int y) const
{
    int result = -1;

    // FUTURE: optimize if needed.
    for (int i =  size.x-1; i >= 0; i--) {
        if (ColorAt(i, y) != Color::clear) {
            result = i;
            break;
        }
    }

    return result;
}

int ExpBitmap::FirstPixelYOnVScanLine(int x) const
{
    int result = -1;

    // FUTURE: optimize if needed.
    for (int i = 0; i < size.y; i++) {
        if (ColorAt(x, i) != Color::clear) {
            result = i;
            break;
        }
    }

    return result;
}

int ExpBitmap::ScanToAnyPixels(ScanLine scan, bool test, Color testColor) const
{
    int result = -1;

    switch (scan) {
        case ScanLine::ScanFromLeft: {
            for (int x = 0; x < size.x; x++) {
                if (AnyPixelsOnVScanLine(x, test, testColor)) {
                    result = x;
                    break;
                }
            }
            break;
        }
        case ScanLine::ScanFromTop: {
            // Scan top to bottom.
            for (int y = 0; y < size.y; y++) {
                if (AnyPixelsOnHScanLine(y, test, testColor)) {
                    result = y;
                    break;
                }
            }
            break;
        }
        case ScanLine::ScanFromRight: {

            // Scan right to left.
            for (int x = size.x - 1; x >= 0; x--) {
                if (AnyPixelsOnVScanLine(x, test, testColor)) {
                    result = x;
                    break;
                }
            }
            break;
        }
        case ScanLine::ScanFromBottom: {
            for (int y = size.y - 1; y >= 0; y--) {
                if (AnyPixelsOnHScanLine(y, test, testColor)) {
                    result = y;
                    break;
                }
            }
            break;
        }
    }

    return result;
}

ExpBitmap* ExpBitmap::NewSubBitmap(RectInt bounds)
{
    ExpBitmap* result = New();
    result->Build(bounds.Width(), bounds.Height(), mPixelFormat);

    int pixelSize = PixelSize();
    int lineSize = pixelSize * bounds.Width();

    int x = bounds.origin.x;
    for (int y = bounds.origin.y; y <= bounds.Bottom(); y++) {

        Color *color = static_cast<Color*>(PixelDataAt(x, y));
        Color *destColorPtr = static_cast<Color*>(result->PixelDataAt(0, y-bounds.origin.y));
        if (nullptr != color && nullptr != destColorPtr) {
            memcpy(destColorPtr, color, lineSize);
        }
    }

    return result;

}


void ExpBitmap::Build(int width, int height, PixelFormat pf = PixelFormat::RGBA8888)
{
    // Get rid of the old bitmap.
    Flush();
    mPixelFormat = pf;

    if (width <= 0 || height <= 0) {
        return;
    }

    size.x = width;
    size.y = height;

    // FUTURE: support more pixel formats.
    uint32_t rowSize = width * PixelSize();
    dataSize = rowSize * height;
    data = (void*) calloc(rowSize * height, 1);
}

/*
    NewBitmap

 */
ExpBitmap* ExpBitmap::NewBitmap()
{
    ExpBitmap* result = nullptr;

    ClassPtr c;
    if (Class::FindClass(CLASS_BITMAP, c)) {
        result = static_cast<ExpBitmap*>(c->FactoryNew(""));
    }
    return result;
}

char* ExpBitmap::NewVPixelLine(int x) const {
    if (x < 0 || x >= Width()) { return nullptr; }

    uint32_t height = Height();
    uint32_t pixelDataSize = GetPixelDataSize();
    uint32_t width = Width();
    uint32_t lineDataSize = pixelDataSize*height;
    if (0 == lineDataSize) { return nullptr; }
    void* pixels = malloc(lineDataSize);

    switch (PixelSize()) {
        case 4: {
            uint32_t* destPixels = static_cast<uint32_t*>(pixels);
            uint32_t* sourcePixels = static_cast<uint32_t*>(GetPixelData(x, 0));
            for (int i = 0; i < height; i++) {

                *destPixels = *(static_cast<uint32_t*>(sourcePixels));
                destPixels++;
                sourcePixels += width;    // Pointer arithmetic
            }
            break;
        }
            // FUTURE: support non 32-bit pixel sizes.
        case 2:
        default:
            PJLog("UNIMPLEMENTED. NewVPIxelLine for PixelSize: %d", PixelSize());
            break;
    }

    return static_cast<char*>(pixels);
}

char* ExpBitmap::NewHPixelLine(int y) const {
    if (y < 0 || y >= Height()) { return nullptr; }

    uint32_t pixelDataSize = GetPixelDataSize();
    uint32_t width = Width();
    uint32_t lineDataSize = pixelDataSize*width;
    if (0 == lineDataSize) { return nullptr; }
    void* pixels = malloc(lineDataSize);

    uint32_t* destPixels = static_cast<uint32_t*>(pixels);
    uint32_t* sourcePixels = static_cast<uint32_t*>(GetPixelData(0, y));

    memcpy(destPixels, sourcePixels, lineDataSize);

    return static_cast<char*>(pixels);
}

bool ExpBitmap::SearchIsOpaque() const {
    if (empty()) { return false; }

    for (uint32_t x = 0; x < Width(); x++) {
        for (uint32_t y = 0; y < Height(); y++) {
            Color color = ColorAt(x, y);
            FColor fColor = ColorToFColor(color);

            if (fColor.alpha != 1.0) {
                return false;
            }
        }
    }
    return true;
}

#pragma mark - EXPERIMENTAL CODE

/*
    EXPERIMENTAL CODE

    This code is experimental. Don't use it, unless you feel brave. It may or may not work.
 */

/*
 CreateUpsampleBicubic

 This code is experimental, and has not been fully tested or verified.

 NOTE: the value of this code is suspect, results aren't that great. Keep for future reference
 and testing.

 */
ExpBitmap* ExpBitmap::CreateUpsampleBicubic(int destWidth, int destHeight)
{
    //ExpBitmap*    result = New();
    //result->Go(destWidth, destHeight, mPixelFormat);
    //
    //float    xScale = (float)Width()/(float)destWidth;
    //float    yScale = (float)Height()/(float)destHeight;
    //
    //for (int x = 0; x < destWidth; x++) {
    //    for (int y = 0; y < destHeight; y++) {
    //        FColor    fColor = GetUpsampleBicubicDestColor(Vector2Int((float)x*xScale, (float)y*yScale), Vector2Int(destWidth, destHeight));
    //        Color    color = FColorToColor(fColor);
    //        result->SetPixelColor(x, y, color);
    //    }
    //}
    //
    //return result;
    return nullptr;

}

int ExpBitmap::IntResamplePixelPos(float width, float x, float destWidth)
{
    // Drop the remainder.
    return (int)floorf(x * destWidth/width);
}

float ExpBitmap::FResamplePixelPos(float width, float x, float destWidth)
{
    return (float)x * destWidth/width;
}

FColor ExpBitmap::ScaledBoxAverageColor(float x, float y, float scale)
{
    FColor    result;

    result.red = 0;
    result.green = 0;
    result.blue = 0;
    result.alpha = 0;

    float s = 1.0f/scale;

    int firstI = static_cast<int>(roundf(x-s/2));
    int lastI = static_cast<int>(roundf(x+s/2));
    int firstJ = static_cast<int>(roundf(y-s/2));
    int lastJ =static_cast<int>( roundf(y+s/2));
    for (int i = firstI; i <= lastI; i++) {
        for (int j = firstJ; j <= lastJ; j++) {
            Color    color = ColorAt(i, j);
            FColor    fColor = ColorToFColor(color);

            result.red += fColor.red;
            result.green += fColor.green;
            result.blue += fColor.blue;
            result.alpha += fColor.alpha;
        }
    }

    int width = lastI - firstI + 1;
    int height = lastJ - firstJ + 1;
    int area = width * height;

    result.red /= (area);
    result.green /= (area);
    result.blue /= (area);
    result.alpha /= (area);

    return result;
}

/*
 CreateDownsampleBoxAverage

 This code is experimental, and has not been fully tested or verified.

 NOTE: the box average downsampling works, but it's not as good as Photoshop.
 FUTURE: run a sharpening filter after downsampling?

 */
ExpBitmap* ExpBitmap::CreateDownsampleBoxAverage(float scale)
{
    // From: http://www.fho-emden.de/~hoffmann/bicubic03042002.pdf
    int destWidth = static_cast<int>(scale*Width());
    int destHeight = static_cast<int>(scale*Height());

    ExpBitmap*    result = New();
    result->Build(destWidth, destHeight, mPixelFormat);

    float    xScale = (float)Width()/(float)destWidth;
    float    yScale = (float)Height()/(float)destHeight;

    for (int x = 0; x < destWidth; x++) {
        for (int y = 0; y < destHeight; y++) {
            FColor fColor = GetScaledBoxAverageColor((float)x*xScale, (float)y*yScale, scale);
            Color color = FColorToColor(fColor);
            result->SetPixelColor(x, y, color);
        }
    }

    return result;
}

FColor ExpBitmap::UpsampleBicubicDestColor(Vector2Int sourcePt, Vector2Int destSize)
{
    //int i = GetIntResamplePixelPos(Width(), sourcePt.x(), destSize.x());
    //int j = GetIntResamplePixelPos(Height(), sourcePt.y(), destSize.y());
    //float    x = GetFResamplePixelPos(Width(), sourcePt.x(), destSize.x());
    //float    y = GetFResamplePixelPos(Height(), sourcePt.y(), destSize.y());
    //float    dx = x - i;
    //float    dy = y - j;
    //
    ////PJLog("Source X, Y: %d, %d", sourcePt.x(), sourcePt.y());
    ////PJLog("Dest X, Y: %d, %d", i, j);
    //
    //float    red = 0.0;
    //float    green = 0.0;
    //float    blue = 0.0;
    //float    alpha = 0.0;
    //
    //for (float m = -1; m <= 2; m++) {
    //    for (float n = -1; n <= 2; n++) {
    //        Color    pixelColor = ColorAt(sourcePt.x() + m, sourcePt.y() + n);
    //        FColor    fColor = ColorToFColor(pixelColor);
    //
    //        red += fColor.red*BicubicWeightFoo(m-dx) * BicubicWeightFoo(dy-n);
    //        green += fColor.green*BicubicWeightFoo(m-dx) * BicubicWeightFoo(dy-n);
    //        blue += fColor.blue*BicubicWeightFoo(m-dx) * BicubicWeightFoo(dy-n);
    //        alpha += fColor.alpha*BicubicWeightFoo(m-dx) * BicubicWeightFoo(dy-n);
    //    }
    //}
    //
    //FColor    result;
    //result.red = fminf(1.0f, red);
    //result.green = fminf(1.0f, green);
    //result.blue = fminf(1.0f, blue);
    //result.alpha = fminf(1.0f, alpha);
    //return result;
    return FColor();
}

float ExpBitmap::BicubicWeightFoo(float x)
{
    float b1 = BicubicP(x + 2);
    b1 = b1*b1*b1;

    float b2 = BicubicP(x + 1);
    b2 = b2*b2*b2*4;

    float b3 = BicubicP(x);
    b3 = b3*b3*b3*6;

    float b4 = BicubicP(x - 1);
    b4 = b4*b4*b4*4;

    float result = (1/6.0f)*(b1-b2+b3-b4);
    return result;
}

float ExpBitmap::BicubicP(float x)
{
    // Clip the coordinates.
    if (x > 0) { return x; }
    return 0;
}


#define PAINT_NODE(x, y, bounds) \
newNodes.insert(FillNode(x, y)); \
floodBitmap->SetPixelColor(x, y, Color::black);  \
if (alter) SetPixelColor(x, y, fillColor); \
bounds.origin.x = min(bounds.origin.x, x); \
bounds.origin.y = min(bounds.origin.y, y); \
bounds.right() = max(bounds.right(), x); \
bounds.Bottom() = max(bounds.Bottom(), y); \
numPixelsPainted++;

// FUTURE: support alpha-multiplication for alpha masks
// FUTURE: support distance searching for pixels at N distance from the fill zone.
uint32_t ExpBitmap::FloodFillAnyAlpha(Vector2Int firstPixel, FColor _fillColor, bool alter, RectInt& bounds)
{
    Color fillColor = FColorToColor(_fillColor);

    // Create an empty bitmap to check the flood fill (avoids infinite loop when altering original bitmap).
    ExpBitmap *floodBitmap = New();
    floodBitmap->Build(size.x, size.y, mPixelFormat);

    set<FillNode> nodes;
    FillNode    firstNode(firstPixel.x(), firstPixel.y());

    bounds.origin.x = INT_MAX;
    bounds.origin.y = INT_MAX;
    bounds.right() = -1;
    bounds.Bottom() = -1;

    uint32_t numPixelsPainted = 0;

    // Need a non-alpha pixel to start the flood fill.
    if (0 == ColorAt(firstNode.x, firstNode.y)) { return numPixelsPainted; }

    floodBitmap->SetPixelColor(firstPixel.x(), firstPixel.y(), Color::black);
    SetPixelColor(firstPixel.x(), firstPixel.y(), fillColor);
    numPixelsPainted++;

    nodes.insert(firstNode);
    while (nodes.size() > 0) {
        // Avoid enumeration mutation by adding to a separate collection.
        set<FillNode> newNodes;
        set<FillNode> removeNodes;

        for (set<FillNode>::iterator i = nodes.begin(); i != nodes.end(); i++) {
            FillNode    node = *i;
            int x = node.x;
            int y = node.y;

            if (Color::black == floodBitmap->ColorAt(x, y)) {
                removeNodes.insert(FillNode(x, y));

                bounds.origin.x = min(bounds.origin.x, x);
                bounds.origin.y = min(bounds.origin.y, y);
                bounds.right() = max(bounds.right(), x);
                bounds.Bottom() = max(bounds.Bottom(), y);
            }

            {

                {
                    int w = x-1;

                    // Check west (inclusive of current node).
                    while (w >= 0) {
                        if (0 == ColorAt(w, y)) {
                            break;
                        }

                        // Check north.
                        if (y > 0) {
                            if (0 != ColorAt(w, y-1) && 0 == floodBitmap->ColorAt(w, y-1)) {
                                PAINT_NODE(w, y-1, bounds);
                            }
                        }

                        // Check south.
                        if ((y+1) < size.y) {
                            if (0 != ColorAt(w, y+1) && 0 == floodBitmap->ColorAt(w, y+1)) {
                                PAINT_NODE(w, y+1, bounds);
                            }
                        }

                        w--;
                    }
                }

                int e = x+1;
                while (e < size.x) {

                    if (0 == ColorAt(e, y)) {
                        break;
                    }

                    // Check north.
                    if (y > 0) {
                        if (0 != ColorAt(e, y-1) && 0 == floodBitmap->ColorAt(e, y-1)) {
                            PAINT_NODE(e, y-1, bounds);
                        }

                    }

                    // Check south.
                    if ((y+1) < size.y) {
                        if (0 != ColorAt(e, y+1) && 0 == floodBitmap->ColorAt(e, y+1)) {
                            PAINT_NODE(e, y+1, bounds);
                        }
                    }

                    e++;
                }
            }
        }

        // Add all the new nodes for the flood fill.
        for (set<FillNode>::iterator i = newNodes.begin(); i != newNodes.end(); i++) {
            nodes.insert(*i);
        }

        // Remove the finished nodes.
        for (set<FillNode>::iterator i = removeNodes.begin(); i != removeNodes.end(); i++) {
            nodes.erase(*i);
        }
    }

    delete floodBitmap;
    return numPixelsPainted;

}

RectInt ExpBitmap::TrimToAnyPixels(int border)
{
    int firstLeft = ScanToAnyPixels(ScanLine::ScanFromLeft);
    int firstTop = ScanToAnyPixels(ScanLine::ScanFromTop);
    int firstRight = ScanToAnyPixels(ScanLine::ScanFromRight);
    int firstBottom = ScanToAnyPixels(ScanLine::ScanFromBottom);
    RectInt result(firstLeft, firstTop, firstRight, firstBottom);
    if (firstLeft < 0 || firstTop < 0 || firstTop < 0 || firstBottom < 0) {
        PJLog("ERROR. TrimToAnyPixels. Bad bitmap.");
        return result;
    }

    firstLeft = std::max(firstLeft, 0);
    firstTop = std::max(firstTop, 0);
    if (firstRight < 0) { firstRight = 0; }
    if (firstBottom < 0) { firstBottom = 0; }

    int width = firstRight-firstLeft+1 + border*2;
    int height = firstBottom-firstTop+1 + border*2;
    width = std::max(width, 1);
    height = std::max(height, 1);

    uint32_t pixelSize = PixelSize();
    char* _data = (char*)calloc(width * height * pixelSize, sizeof(char));
    char* data = _data;
    data += border*pixelSize + border*width*pixelSize;

    for (int y = result.origin.y; y <= result.Bottom(); y++) {
        memcpy(data, PixelDataAt(result.origin.x, y), result.Width()*pixelSize);
        data += width*pixelSize;
    }
    ReplaceData(_data, width, height);

    return result;

}

#endif
